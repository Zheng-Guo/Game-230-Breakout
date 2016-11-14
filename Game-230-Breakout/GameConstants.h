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
const Color Ball_Initial_Color = Color::White;
const float Ball_Next_Level_Acceleration = -0.4;
const int Refresh_Frequency = 100;
const float Refresh_Interval = 1.0/Refresh_Frequency;
const float Deflection_Coefficient = 0.06;
const float Paddle_Speed = 5;
const int Player_Initial_Lives = 3;
const int Stat_Character_Size = 25;
const int Message_Character_Size = 100;
const int Message_X_Position = 300;
const int Message_Y_Position = 400;
const int Level_Name_Character_Size = 80;
const int Level_Name_X_Position = 250;
const int Level_Name_Y_Position = 400;
const int Help_Character_Size = 20;
const int Number_Of_Help_Lines_Per_Page = 26;
const int Help_X_Position = 10;
const int Score_X_Position = 10;
const int Score_Y_Position = 10;
const int Life_X_Position = 10;
const int Life_Y_Position = 40;
const int Black_Curtain_Initial_Height = 10;
const int Black_Curtain_Final_Height = 300;
const int Number_Of_Brick_Per_Row = 9;
const char Config_Folder[] = "config";
const char Config_File_Name[] = "level_config.txt";
const char Help_File_Name[] = "help.txt";
const char Texture_Folder[] = "texture";
const char Texture_Brick_Subfolder[] = "brick";
const char Texture_Ball_Subfolder[] = "ball";
const char Texture_PowerUp_Subfolder[] = "powerup";
const char Animation_Folder[] = "animation";
const char Animation_Water_Subfolder[] = "water";
const char Animation_Fire_Subfolder[] = "fire";
const char Animation_Thunder_Subfolder[] = "thunder";
const char Audio_Folder[] = "audio";
const Color Play_Area_Color = Color(15,170,6);
const Color Normal_Brick_Background_Color = Color::White;
const Color Fire_Brick_Background_Color = Color::Red;
const Color Thunder_Brick_Background_Color = Color(255, 63, 255);
const Color Earth_Upgraded_Background_Color = Color(83, 41, 0);
const Color Water_Upgraded_Background_Color = Color(0, 0, 255);
const Color Due_Upgraded_Background_Color = Color(53, 74, 123);
const int Water_Texture_Number = 16;
const int Fire_Texture_Number = 21;
const float Fire_Ball_Radius = 60;
const int Fire_Ball_Attack_Interval = 15 * Refresh_Frequency;
const int Fire_Ball_Attack_Margin = 5;
const float Fire_Ball_Y_Speed = 400 / Refresh_Frequency;
const int Thunder_Texture_Number = 19;
const float Thunder_Ball_Radius = 60;
const int Thunder_Ball_Attack_Interval = 10 * Refresh_Frequency;
const int Thunder_Ball_Attack_Margin = 5;
const float Thunder_Ball_Y_Speed = 500 / Refresh_Frequency;
const int Player_Stunned_Duration = 2 * Refresh_Frequency;
const Color Paddle_Normal_Color = Color(255,255,0);
const Color Paddle_Stunned_Color = Color::Black;
const int Normal_Brick_Score = 10;
const int Element_Brick_Score = 50;
const int Null_Brick_Score = 100;
const int Null_Cloak_Interval = 10 * Refresh_Frequency;
const int Null_Cloak_Margin = 5;
const int Null_Cloak_Duration = 100*Refresh_Frequency;
const float Power_Up_Display_Radius = 30;
const float Power_Up_Display_X = 20;
const float Power_Up_Display_Y = Play_Area_Height - 80;
const float Power_Up_Selection_Hightlight_Width = 180;
const float Power_Up_Selection_Hightlight_Height = 80;
const Color Power_Up_Selection_Hightlight_Color = Color(247, 139, 21);
const float Power_Up_Display_Interval = 80;
const int Explosion_Texture_Row_Number = 6;
const int Explosion_Texture_Column_Number = 8;
const int Explosion_Speed = Refresh_Frequency / 20;
const int Power_Up_Item_Width = 60;
const int Power_Up_Item_Height = 60;
const float Power_Up_Label_X = 10;
const int Player_Intact = 0;
const int Player_Killed = 1;
const int Player_Stunned = 2;
const int Black_Curtain_Help_Height = 700;
enum Element{None,Normal,Water,Fire,Earth,Wind,Thunder,Null};