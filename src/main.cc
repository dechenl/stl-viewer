#include "SDL3/SDL_events.h"
#define SDL_MAIN_USE_CALLBACKS 1  /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <format>

#include "geometry.h"
#include "parser.h"

#define WIN_H 640
#define WIN_V 480

/* We will use this renderer to draw into this window every frame. */
static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static std::vector<triangle> tris;
static float zoom = 0, zoom0 = 0;
static quaternion rot(0,0,0);
static bool wireframe = false;

static bool need_recalc = true;

static vec3 light = {1,1,1};
static bool ctrl_light = false;

static
struct frag
{
  bool draw;
  float z;
  float shade;
} fbuffer[WIN_H][WIN_V];
static SDL_Texture *streaming_texture;

// 4/3 aspect ratio, far view 1000
// from OpenGL
static mat4 proj[] = {
  // 45 deg
  {
    1.810660, 0.000000, 0.000000, 0.000000,
    0.000000, 2.414213, 0.000000, 0.000000,
    0.000000, 0.000000, -1.002002, -0.200200,
    0.000000, 0.000000, -1.000000, 0.000000
  },

  // 90 deg
  {
    0.750000, 0.000000, 0.000000, 0.000000,
    0.000000, 1.000000, 0.000000, 0.000000,
    0.000000, 0.000000, -1.002002, -0.200200,
    0.000000, 0.000000, -1.000000, 0.000000
  },
};

static int debug_text_y;
void debug_render(const char *text) {
  SDL_RenderDebugText(renderer, 5, debug_text_y, text);
  debug_text_y += 15;
}

void debug_reset() {
  debug_text_y = 5;
}

vec2 get_screen_pos(const vec4 &v4)
{
  return {
    +(v4.x / v4.w + 1) * WIN_H / 2,
    -(v4.y / v4.w - 1) * WIN_V / 2
  };
}

// obsolete
vec2 project(const vec3 & p, float cam_dist) {
  float scale = 200.0f / cam_dist;
  return {
    +p.x * cam_dist/(cam_dist-p.z) * scale + (WIN_H / 2),
    -p.y * cam_dist/(cam_dist-p.z) * scale + (WIN_V / 2),
  };
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("STL Viewer - scroll: zoom | arrow/drag: rotate | space: toggle wireframe | R: reset", WIN_H, WIN_V, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }
  SDL_SetRenderLogicalPresentation(renderer, WIN_H, WIN_V, SDL_LOGICAL_PRESENTATION_LETTERBOX);

  // load model
  bool parse_status = false;
  if (argc > 1) {
    Parser *parser = new STLParser();
    parse_status = parser->Parse(argv[1], tris);
    delete parser;
  }
  else {
    tris.clear();
		tris.push_back(triangle(vec3{0,0.57735,1.73205}, vec3{1,0,0},       vec3{0,1.73205,0}));
		tris.push_back(triangle(vec3{0,0.57735,1.73205}, vec3{0,1.73205,0}, vec3{-1,0,0}));
		tris.push_back(triangle(vec3{0,0.57735,1.73205}, vec3{-1,0,0},      vec3{1,0,0}));
		tris.push_back(triangle(vec3{1,0,0},             vec3{-1,0,0},      vec3{0,1.73205,0}));

		parse_status = true;
  }

  if (!parse_status)
    return SDL_APP_FAILURE;

  // move object to origin
  if (tris.size() > 0)
  {
    float xmin = tris[0][0].x, xmax = xmin,
          ymin = tris[0][0].y, ymax = ymin,
          zmin = tris[0][0].z, zmax = zmin;

    for (const auto &t : tris) {
      for (const auto &v : t) {
        if (v.x < xmin) xmin = v.x;
        else if (v.x > xmax) xmax = v.x;
        if (v.y < ymin) ymin = v.y;
        else if (v.y > ymax) ymax = v.y;
        if (v.z < zmin) zmin = v.z;
        else if (v.z > zmax) zmax = v.z;
      }
    }

    vec3 center = vec3{ xmax + xmin, ymax + ymin, zmax + zmin } / 2.0f;
    for (auto &t : tris) {
      for (auto &v : t) {
        v = v - center;
      }
    }

    // use 1.5f for 90 degree POV
    zoom0 = zoom = std::max(std::max(xmax - xmin, ymax - ymin), zmax - zmin) * 3.0f;
  }

  // texture
  streaming_texture = SDL_CreateTexture(
    renderer, 
    SDL_PIXELFORMAT_RGBA32, 
    SDL_TEXTUREACCESS_STREAMING, 
    WIN_H, 
    WIN_V
  );

  return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;  /* end the program, reporting success to the OS. */
  }

  if (event->type == SDL_EVENT_KEY_DOWN) {

    switch (event->key.key) {
      case SDLK_ESCAPE:
      case SDLK_Q:
        return SDL_APP_SUCCESS;
        break;
      case SDLK_F1:
        SDL_Log("%s", rot.str().c_str());
        break;
      case SDLK_UP:
        {
          rot = quaternion(-0.1,0,0) * rot;
          need_recalc = true;
        }
        break;
      case SDLK_DOWN:
        {
          rot = quaternion(0.1,0,0) * rot;
          need_recalc = true;
        }
        break;
      case SDLK_LEFT:
        {
          rot = quaternion(0,-0.1,0) * rot;
          need_recalc = true;
        }
        break;
      case SDLK_RIGHT:
        {
          rot = quaternion(0,0.1,0) * rot;
          need_recalc = true;
        }
        break;
      case SDLK_R:
        zoom = zoom0;
        rot = quaternion(0,0,0);
        light = vec3(1,1,1);
        wireframe = false;
        need_recalc = true;
        break;
      case SDLK_LCTRL:
        if (event->key.repeat == 0) ctrl_light = true;
        break;
      case SDLK_SPACE:
        wireframe = !wireframe;
        need_recalc = true;
        break;
    }

  } else if (event->type == SDL_EVENT_KEY_UP) {
    switch (event->key.key) {
      case SDLK_LCTRL:
        if (event->key.repeat == 0) ctrl_light = false;
        break;
    }
  } else if (event->type == SDL_EVENT_MOUSE_WHEEL) {

    zoom -= event->wheel.y;
    if (zoom < 1) zoom = 1;
    need_recalc = true;

  } else if (event->type == SDL_EVENT_MOUSE_MOTION) {

    if (event->motion.state & 0b100 ||
        (event->motion.state & 0b1 && ctrl_light)) {
      rotatexz(light, -event->motion.xrel);
      rotateyz(light, event->motion.yrel);
      need_recalc = true;
    }
    else if (event->motion.state & 0b1) {
      quaternion rotx(rad(event->motion.yrel), 0, 0);
      quaternion roty(0, rad(event->motion.xrel), 0);
      rot = roty * rotx * rot;
      need_recalc = true;
    }
  }
  return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
  int tick = SDL_GetTicks();
  static int last_tick = 0;
  static int accu = 0;
  static int fps  = 0;

  // fps cap
  if ((float)(tick - last_tick) / accu < 1000.0 / 60)
    return SDL_APP_CONTINUE;

  // calculate fps every sec
  accu++;
  if (tick - last_tick > 1000) {
    fps = accu;
    accu = 0;
    last_tick = tick;

    // recalc every second
    need_recalc = true;
  }
  
  mat4 view = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, -zoom,
    0, 0, 0, 1
  };
  mat4 model = rot.mat4() * mat4{
    1,0,0,0,
    0,1,0,0,
    0,0,1,0,
    0,0,0,1,
  };
  auto mvp = proj[0] * view * model;

  // calc light direction based on rotation
  vec4 light4(light, 1);
  light4 = rot.inverse().mat4() * light4;
  vec3 rlight = {light4.x, light4.y, light4.z};

  // calc pixel
  const int sample = 4; // 0,2,4 ms no aa
  for (const auto &t : tris) {

    if (!need_recalc)
      break;

    vec4 a4 = {t.a, 1};
    vec4 b4 = {t.b, 1};
    vec4 c4 = {t.c, 1};
    auto pa4 = mvp * a4;
    auto pb4 = mvp * b4;
    auto pc4 = mvp * c4;
    vec2 pa = get_screen_pos(pa4);
    vec2 pb = get_screen_pos(pb4);
    vec2 pc = get_screen_pos(pc4);

    // behind cam
    if (pa4.z < -pa4.w || pb4.z < -pb4.w || pc4.z < -pc4.w)
      continue;

    if (!wireframe)
    {
      // shade calculation

      auto norm = t.normal();
      float shade = 0.5 * (1 + rlight.dot(norm) / rlight.mag() / norm.mag() );

      int minx = (int) std::floor(std::min(std::min(pa.x, pb.x), pc.x));
      int maxx = (int) std::floor(std::max(std::max(pa.x, pb.x), pc.x));
      int miny = (int) std::floor(std::min(std::min(pa.y, pb.y), pc.y));
      int maxy = (int) std::floor(std::max(std::max(pa.y, pb.y), pc.y));

      // boundary
      minx = std::min(std::max(minx, 0), WIN_H -1);
      maxx = std::min(std::max(maxx, 0), WIN_H -1);
      miny = std::min(std::max(miny, 0), WIN_V -1);
      maxy = std::min(std::max(maxy, 0), WIN_V -1);

      for (int y = miny; y <= maxy; ++y)
      {
        for (int x = minx; x <= maxx; ++x)
        {
          for (int n = 0; n < sample; n++)
          {
            // sample point
            vec2 p = {(float)x + 0.5f,(float)y + 0.5f};
            switch (sample)
            {
              case 2:
                p = {(float)x + 0.25f + 0.5f*n, (float)y + 0.25f + 0.5f*n};
                break;
              case 4:
                p = {(float)x + 0.25f + 0.5f*(n%2), (float)y + 0.25f + 0.5f*(n/2)};
                break;
            }
            auto bc = bary(pa, pb, pc, p);
            if ((bc.x + bc.y + bc.z) <= 1.0001)
            {
              auto &cur = fbuffer[x][y];
              float zndc = (bc.x * pa4.z / pa4.w + bc.y * pb4.z / pb4.w + bc.z * pc4.z / pc4.w) /
                           (bc.x / pa4.w + bc.y / pb4.w + bc.z / pc4.w);
              if (cur.draw && zndc >= cur.z)
                continue;

              cur.draw = true;
              cur.shade = shade;
              cur.z = zndc;
              // any sample will do
              break;
            }
          }
        }
      }
    }
    else
    {
      // wireframe calcuation

      vec2 pts[] = {pa,pb,pc};
      for (int i = 0; i < 3; ++i)
      {
        auto &p1 = pts[i], &p2 = pts[(i+1) % 3];
        line2d l1(p1,p2);

        if (std::abs(l1.m()) <= 1)
        {
          int minx = std::floor(std::min(p1.x,p2.x)),
              maxx = std::floor(std::max(p1.x,p2.x));
          minx = std::min(std::max(minx, 0), WIN_H -1);
          maxx = std::min(std::max(maxx, 0), WIN_H -1);
          for (int x = minx; x <= maxx; ++x)
          {
            int y = (int)std::floor(l1.atX(x + 0.5f));
            if (y >= 0 && y < WIN_V)
            {
              auto &cur = fbuffer[x][y];
              cur.draw = true;
              cur.shade = 0;
            }
          }
        }
        else
        {
          // reverse x and y so that func can get value for vertical line
          l1 = line2d({p1.y,p1.x}, {p2.y,p2.x});
          int miny = std::floor(std::min(p1.y,p2.y)),
              maxy = std::floor(std::max(p1.y,p2.y));
          miny = std::min(std::max(miny, 0), WIN_V -1);
          maxy = std::min(std::max(maxy, 0), WIN_V -1);
          for (int y = miny; y <= maxy; ++y)
          {
            // because x,y reversed, atX takes y and outputs x
            int x = (int)std::floor(l1.atX(y + 0.5f));
            if (x >= 0 && x < WIN_H)
            {
              auto &cur = fbuffer[x][y];
              cur.draw = true;
              cur.shade = 0;
            }
          }
        }
      }
    }
  }

  void *pixel_data = NULL;
  int texture_pitch = 0;
  if (need_recalc && SDL_LockTexture(streaming_texture, NULL, &pixel_data, &texture_pitch)) {
    Uint32 *pixels = (Uint32 *)pixel_data;

    // Calculate pitch in terms of pixels rather than bytes
    int pixel_pitch = texture_pitch / sizeof(Uint32); 

    // Overwrite every single pixel with your new frame data
    for (int y = 0; y < WIN_V; y++) {
      for (int x = 0; x < WIN_H; x++) {
        Uint8 r = 0, g = 0, b = 0, a = 255;

        auto &cur = fbuffer[x][y];
        if (cur.draw)
        {
          cur.draw = false;
          r = g = b = cur.shade * 0xff;
        }
        else a = 0;

        // Always use the row index multiplied by the returned pitch!
        pixels[y * pixel_pitch + x] = (a << 24) | (b << 16) | (g << 8) | r;
      }
    }

    SDL_UnlockTexture(streaming_texture);
  }

  /* clear the window to the draw color. */
  SDL_SetRenderDrawColor(renderer, 0, 0x80, 0x80, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);

  SDL_RenderTexture(renderer, streaming_texture, NULL, NULL);

  SDL_SetRenderDrawColor(renderer, 0xff, 0x77, 0x77, SDL_ALPHA_OPAQUE);
  debug_reset();
  debug_render(std::format("fps       : {}", fps).c_str());
  debug_render(std::format("triangles : {}", tris.size()).c_str());
  debug_render(std::format("wireframe : {}", wireframe).c_str());
  debug_render(std::format("zoom      : {}", zoom).c_str());
  debug_render(std::format("rotation  : {}", rot.str()).c_str());
  //debug_render(std::format("light     : {}", light.str()).c_str());

  float x,y;
  SDL_GetMouseState(&x,&y);
  // debug_render(std::format("mouse     : {},{}", x,y).c_str());

  SDL_RenderPresent(renderer);

  need_recalc = false;

  return SDL_APP_CONTINUE;  /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
  /* SDL will clean up the window/renderer for us. */
}
