#include<raylib.h>
#include<cmath>
#define TRAIL_LENGTH 64

struct Photon{
  float r;
  Color color;
  Vector2 dire;
  Vector2 posi;
  float speed;
  Vector2 trail[TRAIL_LENGTH];
  int TrailIndex=0;
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
        float boost = (boss.gravity * 2.0f) / (dist * dist * dist )* boss.radius;
        peas.dire.x += (dx / dist) * boost * dt;
        peas.dire.y += (dy / dist) * boost * dt;
    }
  }
}

// void drag(Photon& peas){
//   float drag=0.98f;
//   peas.dire.x*=drag;
//   peas.dire.y*=drag;
// }
// light doesn't feel the drag in vacuum.  

void resetPhoton(Photon& p, Planet& hole){
    p.posi = {};
    p.dire = {};
    p.speed = ;
    for(int i = 0 ; i<TRAIL_LENGTH; i++) p.trail[i] = p.posi;
    p.TrailIndex = 0;
}

int main(){
  InitWindow(1000,1000, "Black Hole");
  SetTargetFPS(60);

  Photon photo1={5,WHITE,{0.707f,0.707f},{100,100},90};

  Planet hole;
  hole.posi    = {500, 500};
  hole.radius  = 50;
  hole.color   = BLACK;
  hole.gravity = 50000;
  hole.horizon = hole.gravity * 0.006f;
 
  while(WindowShouldClose()==false){

    //movement logic
     
    //attraction towards planet
    gravy(photo1,hole);

    
    photo1.posi.x+=(photo1.dire.x*photo1.speed)*GetFrameTime();
    photo1.posi.y+=(photo1.dire.y*photo1.speed)*GetFrameTime();
    
    float dx = hole.posi.x - photo.posi.x;
    float dy = hole.posi.y - photo.posi.y;
    float dist = sqrtf(dx*dx + dy*dy);
    if(dist < hole.radius){
        resetPhoton(photo1, hole);
    }

    //Trail logic
    photo1.trail[photo1.TrailIndex]=photo1.posi;
    photo1.TrailIndex=(photo1.TrailIndex+1)%TRAIL_LENGTH;
    //Draw here
    BeginDrawing();
    ClearBackground(BLACK);

    // draw event horizon glow
    DrawCircleGradient(hole.posi.x, hole.posi.y,
                        hole.radius * 2.5f,
                        {255, 100, 0, 30},
                        {0, 0, 0, 0});

    // photon sphere ring (where light can theoretically orbit)
    DrawCircleLines(hole.posi.x, hole.posi.y,
                    hole.radius * 1.5f, {255, 140, 0, 120});

    // black hole itself
    DrawCircle(hole.posi.x, hole.posi.y, hole.radius, BLACK);
    DrawCircleLines(hole.posi.x, hole.posi.y,
                    hole.radius, {60, 60, 60, 200});
                        
    //Trail drawing
    for(int i=0;i<TRAIL_LENGTH;i++){
      int index=(photo1.TrailIndex+i) % TRAIL_LENGTH;
      float t=(float)i/TRAIL_LENGTH;
      float alpha=t*t;
      Color c=Fade(photo1.color,alpha);
      DrawCircleV(photo1.trail[index],photo1.r,Fade(photo1.color, alpha));
    }

    // photon
    DrawCircleV(photo1.posi, photo1.r, WHITE);

    EndDrawing();
  }
  CloseWindow();
  return 0;
}