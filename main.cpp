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
};
struct Planet{
  Vector2 posi;
  float radius;
  Color color;
  float gravity;
  float horizon=gravity*6;
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
  }
}
void drag(Photon& peas){
  float drag=0.98f;
  peas.dire.x*=drag;
  peas.dire.y*=drag;
}
int main(){
  InitWindow(1000,1000, "hole");
  SetTargetFPS(30);
  Photon photo1={5,WHITE,{1.0f,1.0f},{100,100},50};
  Planet hole={{500,500},50,RED,50000};
 
  while(WindowShouldClose()==false){

    //movement logic
     
    //attraction towards planet
    gravy(photo1,hole);
    drag(photo1);
    photo1.posi.x+=(photo1.dire.x*photo1.speed)*GetFrameTime();
    photo1.posi.y+=(photo1.dire.y*photo1.speed)*GetFrameTime();
    //Trail logic
    photo1.trail[photo1.TrailIndex]=photo1.posi;
    photo1.TrailIndex=(photo1.TrailIndex+1)%TRAIL_LENGTH;
    //Draw here
    BeginDrawing();
    ClearBackground(BLACK);
    DrawCircle(hole.posi.x,hole.posi.y,hole.radius,hole.color);
    DrawCircle(photo1.posi.x,photo1.posi.y,photo1.r,photo1.color);
    //Trail drawing
    for(int i=0;i<TRAIL_LENGTH;i++){
      int index=(photo1.TrailIndex+i) % TRAIL_LENGTH;
      float t=(float)i/TRAIL_LENGTH;
      float alpha=t+t;
      Color c=Fade(photo1.color,alpha);
      DrawCircleV(photo1.trail[index],photo1.r,c);
    }
    EndDrawing();
  }
  CloseWindow();
  return 0;
}