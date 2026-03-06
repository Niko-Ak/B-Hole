#include<raylib.h>
#include<cmath>
#define TRAIL_LENGTH 64
#define PHOTON_COUNT 64

struct Photon{
  float r;
  Color color;
  Vector2 dire;
  Vector2 posi;
  float speed;
  Vector2 trail[TRAIL_LENGTH];
  int TrailIndex=0;
  Vector2 initposi;
  Vector2 initdire;
  void init(){
    initdire=dire;
    initposi=posi;
  }
}; //hello nithish

struct Planet{
  Vector2 posi;
  float radius;
  Color color;
  float gravity;
  float horizon;
};
void gravy(Photon& peas,Planet& boss){
  float dx=boss.posi.x-peas.posi.x;
  float dy=boss.posi.y-peas.posi.y;
  float dist=sqrtf((dx*dx)+(dy*dy));

  if(dist>boss.radius){
    float force=boss.gravity/(dist*dist);
    float dt=GetFrameTime();

    peas.dire.x+=(dx/dist)*force*dt;
    peas.dire.y+=(dy/dist)*force*dt;

    float photon_sphere = boss.radius * 1.5f;
    if(dist < photon_sphere * 3.0f){
        float boost = (boss.gravity * 2.0f) / (dist * dist * dist )* boss.radius; //this block right here is to normalize the speed of the photon, before this code the photon accelerate when approaching the blackhole but photon does not work like that photon can slow down but cant speed up above the speed of light
        peas.dire.x += (dx / dist) * boost * dt;
        peas.dire.y += (dy / dist) * boost * dt;
    }
  }
  float mag = sqrtf(peas.dire.x*peas.dire.x+peas.dire.y*peas.dire.y);
  if(mag>0.0f){
    peas.dire.x/=mag;
    peas.dire.y/=mag;

  }
}

// void drag(Photon& peas){
//   float drag=0.98f;
//   peas.dire.x*=drag;
//   peas.dire.y*=drag;
// }
// light doesn't feel the drag in vacuum.  

void resetPhoton(Photon& p){
    //p.posi  = {p.initposi.x,p.initposi.y};
    //p.dire  = {p.initdire.x, p.initdire.y};   //uncomment these two lines for reset
    for (int i = 0; i < TRAIL_LENGTH; i++) p.trail[i] = p.posi; // small fix because the resetPhoton forces the photon to go 100,100 even if change the psoition on top
    p.TrailIndex = 0;
}

void DrawGlowCircle(Vector2 pos, float r, Color c, int layers=6){
  for(int i=layers;i>0;i--){
      float size = r + i*3.0f;
      float alpha = 0.08f / i;
      DrawCircleV(pos, size, Fade(c, alpha));
  }
  DrawCircleV(pos, r, c);
}

int main(){
  InitWindow(1000,1000, "Black Hole");
  SetTargetFPS(60);

  //Photon photo1={5,WHITE,{0.707f,0.707f},{50,300},90};photo1.init(); //do this to assign posi and dire to initposi and initdire
  Photon photo1[PHOTON_COUNT];
  for(int i=0;i<PHOTON_COUNT;i++){
        photo1[i].r     = 4;
        photo1[i].color = WHITE;
        photo1[i].speed = 90.0f;
        photo1[i].dire  = {0.707f, 0.707f};
        photo1[i].posi  = {50.0f, 280.0f + i * 10.0f}; // spread vertically   //go tweak the PHOTO_COUNT for funnn
        photo1[i].init();
        for (int j = 0; j < TRAIL_LENGTH; j++) photo1[i].trail[j] = photo1[i].posi;

  }

  Planet hole;
  hole.posi    = {500, 500};
  hole.radius  = 50;
  hole.color   = BLACK;
  hole.gravity = 50000;
  hole.horizon = hole.gravity * 0.006f;
 
  while(!WindowShouldClose()){

    for(int i=0;i<PHOTON_COUNT;i++){
      gravy(photo1[i],hole);
      photo1[i].posi.x+=photo1[i].dire.x*photo1[i].speed*GetFrameTime();
      photo1[i].posi.y+=photo1[i].dire.y*photo1[i].speed*GetFrameTime();

      float dx=hole.posi.x-photo1[i].posi.x;
      float dy=hole.posi.y-photo1[i].posi.y;
      if(sqrtf(dx*dx+dy*dy)<hole.radius)
        resetPhoton(photo1[i]);

      photo1[i].trail[photo1[i].TrailIndex]=photo1[i].posi;
      photo1[i].TrailIndex=(photo1[i].TrailIndex+1)%TRAIL_LENGTH;
    }

    if(IsKeyPressed(KEY_Q)) break;

    BeginDrawing();
    ClearBackground(BLACK);

    DrawCircleGradient(hole.posi.x,hole.posi.y,hole.radius*2.5f,{255,100,0,30},{0,0,0,0});
    DrawCircleLines(hole.posi.x,hole.posi.y,hole.radius*1.5f,{255,140,0,120});
    DrawCircle(hole.posi.x,hole.posi.y,hole.radius,BLACK);
    DrawCircleLines(hole.posi.x,hole.posi.y,hole.radius,{60,60,60,200});

    for(int i=0;i<PHOTON_COUNT;i++){
      for(int j=0;j<TRAIL_LENGTH;j++){
        int index=(photo1[i].TrailIndex+j)%TRAIL_LENGTH;
        float alpha=(float)j/TRAIL_LENGTH;
        alpha*=alpha;
        DrawCircleV(photo1[i].trail[index],photo1[i].r,Fade(photo1[i].color,alpha));
      }
      DrawCircleV(photo1[i].posi,photo1[i].r,WHITE);
    }

    EndDrawing();
  }
  CloseWindow();
  return 0;
}