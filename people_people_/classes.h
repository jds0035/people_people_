#ifndef CLASSES_H
#define CLASSES_H

//so i should make all of the variables private, and have public functions that "print" them. that way nothin gets fucked up


class Triangle{
   public:

	  int x_f;
	  int y_f;
	  int d_max;
	  int x_boxes;
	  int y_boxes;
	  int maxrange;
	  int fired;
	  int health;
	  int power;
	  int shape_type;
	  double health_fraction;
	  int d;
	  int maximum_health;
	  int power_upgrade_counter;
	  int maxrange_upgrade_counter;
	  int health_upgrade_counter;

      Triangle(int x = -1000, int y = -1000) //so this is how you can initialize a triangle to a certain position.. this only happens once (i think)
      {
        x_i = x;
		y_i = y;
		x_f = x;
		y_f = y;
		d_max= 1;
		maxrange= 1;
		health=1;
		power = 1;
		x_boxes = 15; //things to change when board changes
		y_boxes = 15;
		fired = 0;
		shape_type=-1;
		health_fraction = 1;
		d = 0;
		maximum_health=0;
		power_upgrade_counter=0;
		maxrange_upgrade_counter=0;
		health_upgrade_counter = 0;
      }

	  int set_values(int type = -1000)
	  {
		d_max= move[type];
		maxrange= range[type];
		health=health_fraction*maxhealth[type];
		maximum_health=maxhealth[type];
		power=maxpower[type];
		shape_type = type;

		return 0;
	  }

	  int upgrade(int upgrade,int moves_counter)
	  {
		  if (upgrade==0)
		  {health=health*5/4;
		  maximum_health=maximum_health*5/4;
		  health_upgrade_counter +=1;}
		  if (upgrade==1)
		  {d_max=d_max+1;
		  moves_counter +=1;}
		  if (upgrade==2)
		  {power=power+1;
		  power_upgrade_counter +=1;}
		  if (upgrade==3)
		  {maxrange=maxrange+1;
		  maxrange_upgrade_counter +=1;}


		  return moves_counter;
	  }


      int distance()
      {
          return d;
      }

	  int distance_counter()
		  {d=d+1;
	  if (y_i==y_f && x_i==x_f)
	  {d=0;}
	  return 0;
	  }

	  int fire()
	  {if (fired == 0)
	  {fired = 1;}
	  return 0;}
	  
	  int unfire()
	  {fired = 0;
	  return 0;}

	  int inside()
	  {
		  if (x_f>0 && y_f>0 && x_f<=x_boxes && y_f<=y_boxes)
		  {return(1);}
		  else
		  {return 0;}
	  }

	  int addhealth(int add)
	  {
		  health += add;
		  return 0;
	  }



	  int check()
	  {
		  if (inside()==1 && distance() <= d_max)
		  {return(1);}
		  else
		  {return(0);}
	  }

	  int move_up(int g)
	  {

		  y_f += -1;
		  distance_counter();
		  if (check()==0)
		  {y_f += 1;
		  d = d-1;}
		  if (g==2)
		  {d=0;}
		  return 0;
	  }
	  int move_left(int g)
	  {
		  x_f += -1;
		  distance_counter();
		  if (check()==0)
		  {x_f += 1;
		  d = d-1;}
		  if (g==2)
		  {d=0;}
		  return 0;
	  }
	  int move_down(int g)
	  {
		  y_f += 1;
		  distance_counter();
		  if (check()==0)
		  {y_f += -1;
		  d = d-1;}
		  if (g==2)
		  {d=0;}
		  return 0;
	  }
	  int move_right(int g)
	  {
		  x_f +=  1;
		  distance_counter();
		  if (check()==0)
		  {x_f += -1;
		  d = d-1;}
		  if (g==2)
		  {d=0;}
		  return 0;
	  }
	  int reset_testx()
	  {
		  return x_i;
	  } 
	  int reset_testy()
	  {
		  return y_i;
	  }

	  int reset()
	  {
		  x_f= x_i;
		  y_f=y_i;
		  d = 0;
		  return 0;
	  }
	  int take_damage(int damage)
	  {health = health - damage;
	  if (health <= 0)
	  {
	  x_f = 999;
	  x_i = 999;
	  health=0;
	  return health_upgrade_counter;
	  }
	  double health_d = health;
	  double maxhealth_d = maximum_health;
	  health_fraction = health_d/maxhealth_d;
	  return 0;
	  }

		  

	  int fixation()
	  {x_i = x_f;
	  y_i = y_f;
	  d=0;
	  return(0);
	  }

   private:
      int x_i;
	  int y_i;


};

#endif