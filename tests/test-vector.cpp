#include <catch2/catch.hpp>

#include "pyro/vector.h"

TEST_CASE("Vectors", "[vector]")
{
    Pyro::Vector a = Pyro::Vector(0.0f, 1.0f);
    Pyro::Vector b = Pyro::Vector(1.0f, 0.0f);
    Pyro::Vector v1 = Pyro::Vector(1, 2, 3);
    Pyro::Vector v2 = Pyro::Vector(3, 2, 1);
    Pyro::Vector v3 = Pyro::Vector();           // zero vector
    Pyro::Vector v4 = Pyro::Vector(0.2f, 0.1f); // Less than 1 Vector

    REQUIRE(a.x == 0.0f);
    REQUIRE(a.y == 1.0f);
    REQUIRE(b.x == 1.0f);
    REQUIRE(b.y == 0.0f);

    SECTION("Calculations")
    {
        SECTION("Addition")
        {
            SECTION("add b to a")
            {
                Pyro::Vector c = a + b;

                REQUIRE(c.x == 1.0f);
                REQUIRE(c.y == 1.0f);
            }

            SECTION("add a to b")
            {
                Pyro::Vector c = b + a;

                REQUIRE(c.x == 1.0f);
                REQUIRE(c.y == 1.0f);
            }

            SECTION("Adding is commutative")
            {
                REQUIRE(a + b == b + a);
            }
        }

        SECTION("Subtraction")
        {
            SECTION("subtract b from a")
            {
                Pyro::Vector c = a - b;
                REQUIRE(c.x == -1.0f);
                REQUIRE(c.y == 1.0f);
            }

            SECTION("subtract a from b")
            {
                Pyro::Vector c = b - a;
                REQUIRE(c.x == 1.0f);
                REQUIRE(c.y == -1.0f);
            }

            SECTION("Subtraction is not commutative")
            {
                REQUIRE_FALSE(a - b == b - a);
            }
        }

        SECTION("Multiplication")
        {
            Pyro::Vector v3 = v2 * 2.0f;
            REQUIRE(v3.x == 6);
            REQUIRE(v3.y == 4);
            REQUIRE(v3.z == 2);
        }

        SECTION("DIVISION")
        {
            Pyro::Vector v3 = v2 / 2.0f;
            REQUIRE(v3.x == 1.5);
            REQUIRE(v3.y == 1);
            REQUIRE(v3.z == 0.5);
        }
    }

    SECTION("Properties")
    {
        SECTION("Get magnitude")
        {
            float v = Pyro::Vector(0, 0, 0).mag();
            REQUIRE(v == 0.0f);

            v = Pyro::Vector(0, 1, 0).mag();
            REQUIRE(v == 1.0f);

            v = Pyro::Vector(4, 3, 0).mag();
            REQUIRE(v == 5.0f);

            REQUIRE((Pyro::Vector(1.0f, 0.0f)).mag() == 1.0f);
            REQUIRE((Pyro::Vector(0.0f, 1.0f)).mag() == 1.0f);

            REQUIRE((Pyro::Vector(3.0f, 4.0f)).mag() == 5.0f);
            REQUIRE((Pyro::Vector(5.0f, 12.0f)).mag() == 13.0f);

            REQUIRE((Pyro::Vector(8.0f, 15.0f)).mag() == 17.0f);
            REQUIRE((Pyro::Vector(7.0f, 24.0f)).mag() == 25.0f);
        }

        SECTION("Set magnitude")
        {
            Pyro::Vector v = Pyro::Vector(4, 3, 2);
            REQUIRE(v.mag() != 4.0f);
            v = v.setmag(4);
            REQUIRE(v.mag() == 4.0f);
        }

        SECTION("Normalize")
        {
            Pyro::Vector t = v2.normalize();
            REQUIRE(t.mag() == Approx(1.0f));

            t = v1.normalize();
            REQUIRE(t.mag() == Approx(1.0f));

            t = v3.normalize();
            REQUIRE(t.mag() == Approx(0.0f));

            t = v4.normalize();
            REQUIRE(t.mag() == Approx(1.0f));
        }

        SECTION("Limit")
        {
            Pyro::Vector v = Pyro::Vector(1.0f, 0.0f, 0.0f);

            v = v.limit(2.0f);
            REQUIRE(v.mag() == 1.0f);

            v = v.limit(1.0f);
            REQUIRE(v.mag() == 1.0f);

            v = v.limit(0.5f);
            REQUIRE(v.mag() == 0.5f);
        }

        SECTION("Rotation")
        {
            Pyro::Vector v = Pyro::Vector(1.0f, 0.0f);
            v = v.rotate(M_PI);
            printf("x, y: %f, %a\n", v.x, v.y);
            REQUIRE(v.mag() == Approx(1.0f));
            REQUIRE(v.x == Approx(-1.0f));
            REQUIRE(v.y == Approx(0.0f).margin(.0000001f));

            v = v.rotate(M_PI / 2.0);
            REQUIRE(v.mag() == Approx(1.0f));
            REQUIRE(v.x == Approx(0.0f).margin(.000001f));
            REQUIRE(v.y == Approx(-1.0f).margin(.0000001f));
        }
    }
}

TEST_CASE("Vectors have angle between them", "[vector]")
{
    float angle = Pyro::Vector(0.0f, 10.0f).angle(Pyro::Vector(10.0f, 0.0f));
    REQUIRE(angle - (3.1415926 / 2.0f) < 0.0001f);
    angle = Pyro::Vector(0.f, 10.f).angle(Pyro::Vector(0.f, 10.f));
    REQUIRE(angle < 0.0001);

    angle = Pyro::Vector(0.f, -10.f).angle(Pyro::Vector(0.f, 10.f));
    REQUIRE(angle - 3.1415926 < 0.0001f);

    angle = Pyro::Vector(0.f, 10.f).angle(Pyro::Vector(10.f, 10.f));
    REQUIRE(angle - (3.1415926 / 4.) < 0.0001);
}

TEST_CASE("Vectors have a heading", "[vector]")
{
    REQUIRE(Pyro::Vector(1.0f, 0.0f).heading() == 0.0f);
    REQUIRE(Pyro::Vector(0.0f, 1.0f).heading() == Approx(M_PI_2));
    REQUIRE(Pyro::Vector(-1.0f, 0.0f).heading() == Approx(M_PI));
    REQUIRE(Pyro::Vector(0.0f, -1.0f).heading() == Approx(-M_PI_2));
}

TEST_CASE("Vectors can be initialized in various ways", "[vector]")
{
    SECTION("Zero initialisation")
    {
        Pyro::Vector v1 = Pyro::Vector();
        REQUIRE(v1.x == 0.0f);
        REQUIRE(v1.y == 0.0f);
    }
    SECTION("Normal initialization")
    {
        Pyro::Vector v1 = Pyro::Vector(1.0f, 2.0f, 3.0f);
        Pyro::Vector v2 = Pyro::Vector(3.0f, 2.0f, 1.0f);

        REQUIRE(v1.x == 1.0f);
        REQUIRE(v2.x == 3.0f);
    }

    SECTION("Random 2D")
    {
        unsigned int tests = 1000000;
        double values[tests];
        double sum = 0.0f;
        for (uint i = 0; i < tests; i++)
        {

            Pyro::Vector v = Pyro::Vector::random2d();
            REQUIRE(abs(v.mag() - 1.0f) < 0.00001f);
            values[i] = (v.heading() + M_PI);
            sum += values[i];
        }

        double mean = sum / tests;

        REQUIRE(mean == Approx(M_PI).margin(.01));
    }

    SECTION("Random 3D")
    {
        Pyro::Vector v = Pyro::Vector::random3d();
        REQUIRE(abs(v.mag() - 1.0f) < 0.00001f);
    }
}