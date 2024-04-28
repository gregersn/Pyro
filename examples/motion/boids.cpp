/**
 * Flocking
 * by Daniel Shiffman.
 *
 * Converted from Java/Processing to C++ by Greger Stolt Nilsen.
 *
 * An implementation of Craig Reynold's Boids program to simulate
 * the flocking behavior of birds. Each boid steers itself based on
 * rules of avoidance, alignment, and coherence.
 *
 * Click the mouse to add a new boid.
 */

#include "pyro/pyro.h"
#include <iostream>
#include <vector>

using namespace Pyro;

// The Boid class

class Boid
{

public:
  Vector position;
  Vector velocity{Vector::random2d()};
  Vector acceleration{};
  float r{2.0f};
  float maxforce{0.03f}; // Maximum steering force
  float maxspeed{2.0f};  // Maximum speed

  Boid(float x, float y) : position{x, y} {};

  void run(const std::vector<Boid> &boids)
  {
    flock(boids);
    update();
    borders();
    render();
  };

  void applyForce(Vector force)
  {
    // We could add mass here if we want A = F / M
    acceleration += force;
  };

  // We accumulate a new acceleration each time based on three rules
  void flock(const std::vector<Boid> &boids)
  {
    Vector sep = separate(boids); // Separation
    Vector ali = align(boids);    // Alignment
    Vector coh = cohesion(boids); // Cohesion
    // Arbitrarily weight these forces
    sep *= 1.5;
    ali *= 1.0;
    coh *= 1.0;
    // Add the force vectors to acceleration
    applyForce(sep);
    applyForce(ali);
    applyForce(coh);
  };

  // Method to update position
  void update()
  {
    // Update velocity
    velocity += acceleration;
    // Limit speed
    // velocity = velocity.limit(maxspeed);
    position += velocity;
    // Reset accelertion to 0 each cycle
    acceleration *= 0.0;
  };

  // A method that calculates and applies a steering force towards a target
  // STEER = DESIRED MINUS VELOCITY
  Vector seek(Vector target)
  {
    Vector desired = target - position; // A vector pointing from the position to the target
    // Scale to maximum speed
    desired = desired.normalize();
    desired *= maxspeed;

    // Above two lines of code below could be condensed with new Vector setMag() method
    // Not using this method until Processing.js catches up
    // desired.setMag(maxspeed);

    // Steering = Desired minus Velocity
    Vector steer = (desired - velocity);
    steer = steer.limit(maxforce); // Limit to maximum steering force
    return steer;
  };

  void render()
  {
    // Draw a triangle rotated in the direction of velocity
    float theta = velocity.heading() + radians(90);
    fill(200, 100);
    stroke(255);
    pushmatrix();
    translate(position.x, position.y);
    rotate(theta);
    beginshape(TRIANGLES);
    vertex(0, -r * 2);
    vertex(-r, r * 2);
    vertex(r, r * 2);
    endshape();
    popmatrix();
  };

  // Wraparound
  void borders()
  {
    if (position.x < -r)
      position.x = width + r;
    if (position.y < -r)
      position.y = height + r;
    if (position.x > width + r)
      position.x = -r;
    if (position.y > height + r)
      position.y = -r;
  };

  // Separation
  // Method checks for nearby boids and steers away
  Vector separate(const std::vector<Boid> &boids)
  {
    float desiredseparation = 25.0f;
    Vector steer{0, 0, 0};
    int count = 0;
    // For every boid in the system, check if it's too close
    for (Boid other : boids)
    {
      float d = position.dist(other.position);
      // If the distance is greater than 0 and less than an arbitrary amount (0 when you are yourself)
      if ((d > 0) && (d < desiredseparation))
      {
        // Calculate vector pointing away from neighbor
        Vector diff = (position - other.position);
        diff = diff.normalize();
        diff = diff.div(d); // Weight by distance
        steer += diff;
        count++; // Keep track of how many
      }
    }
    // Average -- divide by how many
    if (count > 0)
    {
      steer = steer.div((float)count);
    }

    // As long as the vector is greater than 0
    if (steer.mag() > 0)
    {
      // Implement Reynolds: Steering = Desired - Velocity
      steer = steer.setmag(maxspeed);
      steer -= (velocity);
      steer = steer.limit(maxforce);
    }
    return steer;
  };

  // Alignment
  // For every nearby boid in the system, calculate the average velocity
  Vector align(const std::vector<Boid> &boids)
  {
    float neighbordist = 50;
    Vector sum{0, 0, 0};
    int count = 0;
    for (Boid other : boids)
    {
      float d = position.dist(other.position);
      if ((d > 0) && (d < neighbordist))
      {
        sum += (other.velocity);
        count++;
      }
    }
    if (count > 0)
    {
      sum = sum.div((float)count);
      // Implement Reynolds: Steering = Desired - Velocity
      sum = sum.setmag(maxspeed);
      Vector steer = (sum - velocity);
      steer = steer.limit(maxforce);
      return steer;
    }
    else
    {
      return Vector(0, 0);
    }
  };

  // Cohesion
  // For the average position (i.e. center) of all nearby boids, calculate steering vector towards that position
  Vector cohesion(std::vector<Boid> boids)
  {
    float neighbordist = 50;
    Vector sum = Vector(0, 0); // Start with empty vector to accumulate all positions
    int count = 0;
    for (Boid other : boids)
    {
      float d = position.dist(other.position);
      if ((d > 0) && (d < neighbordist))
      {
        sum += (other.position); // Add position
        count++;
      }
    }
    if (count > 0)
    {
      sum = sum.div(count);
      return seek(sum); // Steer towards the position
    }
    else
    {
      return Vector(0, 0);
    }
  }
};

// The Flock (a list of Boid objects)

class Flock
{
  std::vector<Boid> boids{};

public:
  Flock()
  {
  }

  void run()
  {
    for (std::vector<Boid>::iterator it = boids.begin(); it != boids.end(); ++it)
    {

      Boid &b = *it;
      b.run(boids); // Passing the entire list of boids to each boid individually
    }
  }

  void addBoid(Boid b)
  {
    boids.push_back(b);
  }
};

Flock flock{};

// Add a new boid into the System
void mousePressed()
{
  flock.addBoid(Boid(get_mousex(), get_mousey()));
}

void setup()
{
  size(640, 360);
  set_mousepressed(mousePressed);

  // Add an initial set of boids into the system
  for (int i = 0; i < 150; i++)
  {
    flock.addBoid(Boid{width / 2.f, height / 2.f});
  }
}

void draw()
{
  background(50);
  flock.run();
}

int main(int argc, char **argv)
{
  run(setup, draw);
  return 0;
}
