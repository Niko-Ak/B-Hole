#include<raylib.h>
#include<cmath>
#define TRAIL_LENGTH 64
#define PHOTON_COUNT 120

// position of photon in given time
struct State{
  Vector2 position;
  Vector2 velocity;
};

// rate of change
struct Derivative{
  Vector2 velocity;
  Vector2 acceleration;
};

struct Planet{
  Vector2 posi;
  float radius;
  Color color;
  float gravity;
  float horizon;
};

struct Photon{
  float r;
  Color color;
  // Vector2 dire;
  // Vector2 posi;
  State state;
  float speed;
  Vector2 trail[TRAIL_LENGTH];
  int TrailIndex=0;
  Vector2 initposi;
  Vector2 initdire;
  float redshift;
  void init(){
    initdire = state.velocity; // Replaced dire 
    initposi = state.position; // Replaced posi 
  }
}; //hello nithish

Derivative eval(State init_state, Planet boss){
  Derivative output;
  output.velocity = init_state.velocity;
  Vector2 acc = {0.0f, 0.0f};

  // find distance vector
  float dx = boss.posi.x - init_state.position.x;
  float dy = boss.posi.y - init_state.position.y;
  
  // cal the exact distance using Pythagrorean theorem
  float dist = sqrtf(dx * dx + dy * dy);

  // divide by zero check
  if(dist > boss.radius * 0.1f){

    // Schwaszschild 1/r^3
    float force = (boss.gravity * boss.radius * 2.5f) / (dist * dist * dist);
    
    acc.x = (dx / dist) * force;
    acc.y = (dy / dist) * force;
  } 
  output.acceleration = acc; 
  return output;
}




State integrate(State init_state, float dt, Planet boss){
 
  Derivative k1 = eval(init_state, boss);

  State state_k2;
  state_k2.position.x = init_state.position.x + k1.velocity.x * (dt * 0.5f);
  state_k2.position.y = init_state.position.y + k1.velocity.y * (dt * 0.5f);
  state_k2.velocity.x = init_state.velocity.x + k1.acceleration.x * (dt * 0.5f);
  state_k2.velocity.y = init_state.velocity.y + k1.acceleration.y * (dt * 0.5f);

  Derivative k2 = eval(state_k2, boss);

  State state_k3;
  state_k3.position.x = init_state.position.x + k2.velocity.x * (dt * 0.5f);
  state_k3.position.y = init_state.position.y + k2.velocity.y * (dt * 0.5f);
  state_k3.velocity.x = init_state.velocity.x + k2.acceleration.x * (dt * 0.5f);
  state_k3.velocity.y = init_state.velocity.y + k2.acceleration.y * (dt * 0.5f);

  Derivative k3 = eval(state_k3, boss);

  State state_k4;
  state_k4.position.x = init_state.position.x + k3.velocity.x * (dt * 0.5f);
  state_k4.position.y = init_state.position.y + k3.velocity.y * (dt * 0.5f);
  state_k4.velocity.x = init_state.velocity.x + k3.acceleration.x * (dt * 0.5f);
  state_k4.velocity.y = init_state.velocity.y + k3.acceleration.y * (dt * 0.5f);

  Derivative k4 = eval(state_k4, boss);

  State final_state;

  float dx = (k1.velocity.x + 2.0f * k2.velocity.x + 2.0f * k3.velocity.x + k4.velocity.x) / 6.0f;
  float dy = (k1.velocity.y + 2.0f * k2.velocity.y + 2.0f * k3.velocity.y + k4.velocity.y) / 6.0f;
  float dax = (k1.acceleration.x + 2.0f * k2.acceleration.x + 2.0f * k3.acceleration.x + k4.acceleration.x) / 6.0f;
  float day = (k1.acceleration.y + 2.0f * k2.acceleration.y + 2.0f * k3.acceleration.y + k4.acceleration.y) / 6.0f;

  final_state.position.x = init_state.position.x + dx * dt;
  final_state.position.y = init_state.position.y + dy * dt;
  final_state.velocity.x = init_state.velocity.x + dax * dt;
  final_state.velocity.y = init_state.velocity.y + day * dt;

  return final_state;
}






// void gravy(Photon& peas,Planet& boss){
//   float dx=boss.posi.x-peas.posi.x;
//   float dy=boss.posi.y-peas.posi.y;
//   float dist=sqrtf((dx*dx)+(dy*dy));

//   if(dist>boss.radius){
//     float force=boss.gravity/(dist*dist);
//     float dt=GetFrameTime();

//     peas.dire.x+=(dx/dist)*force*dt;
//     peas.dire.y+=(dy/dist)*force*dt;

//     float photon_sphere = boss.radius * 1.5f;
//     if(dist < photon_sphere * 3.0f){
//         float boost = (boss.gravity * 2.0f) / (dist * dist * dist )* boss.radius; //this block right here is to normalize the speed of the photon, before this code the photon accelerate when approaching the blackhole but photon does not work like that photon can slow down but cant speed up above the speed of light
//         peas.dire.x += (dx / dist) * boost * dt;
//         peas.dire.y += (dy / dist) * boost * dt;
//     }
//   }

//   float mag = sqrtf(peas.dire.x*peas.dire.x+peas.dire.y*peas.dire.y);
//   if(mag>0.0f){
//     peas.dire.x/=mag;
//     peas.dire.y/=mag;
//   }
// }




// void drag(Photon& peas){
//   float drag=0.98f;
//   peas.dire.x*=drag;
//   peas.dire.y*=drag;
// }
// light doesn't feel the drag in vacuum.  




void resetPhoton(Photon& p){
    p.state.position = {p.initposi.x, p.initposi.y}; 
    p.state.velocity = {p.initdire.x, p.initdire.y}; 
    for (int i = 0; i < TRAIL_LENGTH; i++) p.trail[i] = p.state.position; 
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




void UpdateRedshift(Photon& peas,Planet& boss){
  float dx=boss.posi.x-peas.state.position.x;
  float dy=boss.posi.y-peas.state.position.y;

  float dist=sqrtf(dx*dx+dy*dy);
  float influ=boss.radius*3.0f;
  if(dist<influ){
    peas.redshift=1.0f-(dist-boss.radius)/(influ-boss.radius);
    peas.redshift=fmaxf(0.0f,fminf(1.0,peas.redshift));
  }
  else peas.redshift=0.0f;
  peas.color={255,(unsigned char)(255*(1.0f-peas.redshift*0.8f)),(unsigned char)(255*(1.0f-peas.redshift)), 255};
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
        photo1[i].state.velocity  = {1.0f * photo1[i].speed, 0.0f};
        photo1[i].state.position  = {50.0f, 300.0f + i * 3.5f}; // spread vertically   //go tweak the PHOTO_COUNT for funnn
        photo1[i].init();
        photo1[i].redshift=0.0f;
        for (int j = 0; j < TRAIL_LENGTH; j++) photo1[i].trail[j] = photo1[i].state.position;
  }

  Planet hole;
  hole.posi    = {500, 500};
  hole.radius  = 50;
  hole.color   = BLACK;
  hole.gravity = 8000000;
  hole.horizon = hole.gravity * 0.006f;
 
  while(!WindowShouldClose()){

    for(int i=0;i<PHOTON_COUNT;i++){
      // gravy(photo1[i],hole);
      // UpdateRedshift(photo1[i],hole);
      // photo1[i].state.position.x+=photo1[i].dire.x*photo1[i].speed*GetFrameTime();
      // photo1[i].state.position.y+=photo1[i].dire.y*photo1[i].speed*GetFrameTime();

      photo1[i].state = integrate(photo1[i].state, GetFrameTime(), hole);

      float vx = photo1[i].state.velocity.x;
      float vy = photo1[i].state.velocity.y;
      float current_speed = sqrtf(vx * vx + vy * vy);

      if(current_speed > 0.0f){
        photo1[i].state.velocity.x = (vx / current_speed) * photo1[i].speed;
        photo1[i].state.velocity.y = (vy / current_speed) * photo1[i].speed;
      }

      UpdateRedshift(photo1[i],hole);

      float dx=hole.posi.x-photo1[i].state.position.x;
      float dy=hole.posi.y-photo1[i].state.position.y;
      if(sqrtf(dx*dx+dy*dy)<hole.radius)
        resetPhoton(photo1[i]);

      photo1[i].trail[photo1[i].TrailIndex]=photo1[i].state.position;
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
      DrawCircleV(photo1[i].state.position,photo1[i].r,photo1[i].color);
    }

    EndDrawing();
  }
  CloseWindow();
  return 0;
}











