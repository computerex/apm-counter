#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <vector>

class Mouse
{
  public:
    Mouse();
    void timestep(unsigned int ctime);
    int curspeed() { return currentspeed; }; // pixels per second
    int avgspeed() { return averagespeed; };
  private:
    POINT lastpoint;
    unsigned int last_time, start_time;
    int currentspeed, averagespeed;
    std::vector<int> samples;
};

Mouse::Mouse()
{
  last_time=currentspeed=averagespeed=start_time=0;
  lastpoint.x=lastpoint.y=0;
};

void Mouse::timestep(unsigned int ctime)
{
  double dt = (ctime-last_time);
  if ( dt == 0 ) return;
  POINT curpos, delta;
  GetCursorPos(&curpos);
  delta.x=curpos.x-lastpoint.x;
  delta.y=curpos.y-lastpoint.y;
  lastpoint=curpos;
  currentspeed=sqrt(delta.x*delta.x+delta.y*delta.y)/dt*1000;
  //printf("%d %.2f\n",currentspeed, dt);
  last_time=ctime;
  samples.push_back(currentspeed);
  if ( ctime-start_time >= 1000 )
  {
    averagespeed=0;
    for ( int i = 0; i < samples.size(); i++ )
      averagespeed+=samples[i];
    averagespeed/=samples.size();
    start_time=ctime;
    samples.clear();
  }
};

class Key
{
  public:
    Key(int keycode);
    void timestep(unsigned int ctime);
    int pps() { return ppsrecord; } // press per second
  private:
    unsigned int start_time;
    int counter, key, ppsrecord;
    bool down;
};

Key::Key(int keycode)
{
  this->key=keycode;
  start_time=counter=down=ppsrecord=0;
}
void Key::timestep(unsigned int ctime)
{
  if ( GetAsyncKeyState(key) ) {
      if ( !down ){
        down=true;
        counter++;
      }
    }
    else{
      down=false;
    }
    if ( ctime-start_time >= 1000 ){
      start_time=ctime;
      //printf("%d keypresses/second \n", counter);
      ppsrecord=counter;
      counter=0;
    }
}

int main()
{
  FILE * file = fopen("apmcount", "w");
  if ( file == 0 ){
       printf("unable to create output file, terminating\n");
       return 1;
  }
  std::vector<Key> keys;
  for ( int i = 0x30; i <= 0x5A; i++ )
      keys.push_back(Key(i)); 
  keys.push_back(Key(VK_LBUTTON));
  keys.push_back(Key(VK_RBUTTON));
  keys.push_back(Key(VK_SHIFT));
  
  Mouse mouse;
  unsigned int ctime, last_time=GetTickCount(), count;
  while ( true )
  {
    Sleep(10);
    ctime=GetTickCount();
    for ( unsigned int i = 0; i < keys.size(); i++ )
        keys[i].timestep(ctime);
    mouse.timestep(ctime);
    if ( ctime-last_time >= 1000 ){
         count=0;
         for ( unsigned int i = 0; i < keys.size(); i++ )
             count = count + keys[i].pps();
    printf("%d m%d\n", count, mouse.avgspeed());
      fprintf(file, "%d m%d\n", count, mouse.avgspeed());
      last_time=ctime;
    }
  }
  fclose(file);
  return 0;
}
