#include <iostream>
using namespace std;

float t_shrink_to_mini_map = 4;

class translate
{
public:
    float translate_x(float);
    float translate_y(float);
    float translate_x_to_minimap(float);
    float translate_y_to_minimap(float, float);
};

float translate::translate_x(float x)
{
    return ((x / (float)450) - (float(16) / (float)9));
}

float translate::translate_y(float y)
{
    return ((y / (float)450) - 1);
}

float translate::translate_x_to_minimap(float x)
{

    return (x / t_shrink_to_mini_map);
}

float translate::translate_y_to_minimap(float y, float width)
{
    return ((y / t_shrink_to_mini_map) + width - width / t_shrink_to_mini_map);
}