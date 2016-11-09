#pragma once
#include <SFML\Graphics.hpp>
using namespace sf;

const float Window_Width = 920;
const float Window_Height = 900;
const float Play_Area_Width = 720;
const float Play_Area_Height = 900;
const int Play_Area_X_Position = 200;
const int Play_Area_Y_Position = 0;
const float Brick_Width = 80;
const float Brick_Height = 80;
const int Brick_Duribility = 16;
const float Paddle_Width = 100;
const float Paddle_Height = 20;
const float Ball_Radius = 15;
const float Ball_Initial_Speed = -2;
const float Ball_Next_Level_Acceleration = -0.1;
const int Refresh_Frequency = 100;
const float Refresh_Interval = 1.0/Refresh_Frequency;
const float Deflection_Coefficient = 0.06;
const float Paddle_Speed = 5;
const int Player_Initial_Lives = 3;
const int Stat_Character_Size = 25;
const int Score_X_Position = 10;
const int Score_Y_Position = 10;
const int Life_X_Position = 10;
const int Life_Y_Position = 40;
const int Number_Of_Brick_Per_Row = 9;
const char Config_File_Name[] = "level_config.txt";
const Color Play_Area_Color = Color::Green;
enum Element{Water=2,Fire,Earth,Wind,Thunder,Null};