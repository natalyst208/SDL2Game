
#include "stdafx.h"
#include "ThreatObject.h"

ThreatObject::ThreatObject()
{
	width_frame_ = 0;
	height_frame_ =0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 0.0;
	y_pos_ = 0.0;
	on_ground_ =0 ;
	comebacktime = 0;
	frame_ =0;
	animation_left = 0;
	animation_right = 0;
	input_type_.left_ = 0;
	type_move_ = REMAIN_THREAT;
}
ThreatObject::~ThreatObject()
{

}

bool ThreatObject::LoadImg(std::string path, SDL_Renderer* screen)
{

	bool ret = BaseObject::LoadImg(path,screen);
	if (ret == true )
	{
		width_frame_ = rect_.w / THREAT_FRAME_NUM;
		height_frame_ = rect_.h;
	}
	return ret;
}

void ThreatObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0 )
	{
		for (int i = 0; i < THREAT_FRAME_NUM; i++)
		{
		frame_clip_[i].x = i * width_frame_;
		frame_clip_[i].y = 0;
		frame_clip_[i].w = width_frame_;
		frame_clip_[i].h = height_frame_;

		}
	}

}
void ThreatObject::Show(SDL_Renderer* des)
{
	if (comebacktime == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;
		frame_++;
		if (frame_ >=8 )
		{
			frame_ = 0 ;
		}

		SDL_Rect* currentClip = &frame_clip_[frame_];
		SDL_Rect rendQuad = {rect_.x,rect_.y,width_frame_,height_frame_};
		SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
	}

}

void ThreatObject::DoPlayer(Map& tMap)
{
	if (comebacktime == 0 )
	{
		x_val_ =0;
		y_val_ += THREAT_GRAVITY_SPEED;
		if (y_val_ >= THREAT_MAX_FALL_SPEED)
		{
			y_val_ = THREAT_MAX_FALL_SPEED;
		}
		if (type_move_ == MOVE_THREAT)
		{
		if (input_type_.left_ == 1)
		{
			x_val_ -= THREAT_SPEED;
		}
		else 
		{
			x_val_ += THREAT_SPEED;
		}
		}

		CheckMap(tMap);
	}
	else if (comebacktime > 0 )
	{
		comebacktime --;
		if (comebacktime == 0)
		{
			x_val_ = 0;
			y_val_ = 0 ;
			if (x_pos_ > 256)
			{
				x_pos_ -= 256;
				animation_left -= 256;
				animation_right -= 256;
			}
			else {x_pos_ = 0 ;}
			y_pos_ = 0;
			input_type_.left_ = 1;
		}

	}


}

void ThreatObject::CheckMap(Map& map_data)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	// kiem_tra_theo_chieu_ngang
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

	x1 =( x_pos_ + x_val_ ) / TILE_SIZE;
	x2 =( x_pos_ + x_val_ + width_frame_ -1 ) / TILE_SIZE;

	y1 =( y_pos_ ) / TILE_SIZE;
	y2 =( y_pos_  + height_min -1 ) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y )
	{
		if (x_val_ > 0)
		{
			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];

				if ((val1 != 0 && val1 != COIN)  
					|| (val2 != 0 && val2 != COIN ) )
				{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ +1;
				x_val_ = 0 ;
				}

		}
		else if (x_val_ < 0 )
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y2][x1];
				if ((val1 != 0 && val1 != COIN) || (val2 != 0 && val2 != COIN ))
				{
				x_pos_ = (x1 + 1 )* TILE_SIZE;
				x_val_ = 0 ;
				}
			
		}


	}

	// kiem tra theo chieu doc
	int width_min = width_frame_ < TILE_SIZE ? width_frame_ :TILE_SIZE;
	x1 = (x_pos_ ) / TILE_SIZE;
	x2 = (x_pos_+ width_min ) / TILE_SIZE;

	y1 = (y_pos_ + y_val_ )/ TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ -1 )/ TILE_SIZE; 

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y )
	{
		if (y_val_ > 0 )
		{
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];
				if ((val1 != 0 && val1 != COIN) || (val2 != 0 && val2 != COIN))
				{
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (height_frame_ +1);
				y_val_ = 0;
				on_ground_ = true;
				}
		}
		else if (y_val_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];
			if ((val1 != 0 && val1 != COIN) || (val2 != 0 && val2 != COIN))
			{
				y_pos_ = (y1 + 1 ) *TILE_SIZE;
				y_val_ = 0 ;
			}

		}
	}
	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ <0)
		x_pos_ = 0;
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}

	if (y_pos_ > map_data.max_y_)
	{
		comebacktime = 60;
	}


}
void ThreatObject::ImpMoveType(SDL_Renderer* screen)
{
	if (type_move_ == REMAIN_THREAT)
	{

	}
	else
	{
		if ( on_ground_ ==true )
		{
			if (x_pos_ > animation_right)
			{
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				LoadImg("img//threat_left.png",screen);
			}
			else if ( x_pos_ < animation_left)
			{
				input_type_.right_ = 1;
				input_type_.left_ = 0;
				LoadImg("img//threat_right.png",screen);
			}

		}
		else 
		{
			if (input_type_.left_ == 1 )
			{
				LoadImg("img//threat_left.png",screen);
			}
			else
			{
				LoadImg("img//threat_right.png",screen);
			}


		}


	}


}


