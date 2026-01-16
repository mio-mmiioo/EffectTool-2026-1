#include "Stage.h"
#include <assert.h>
#include "../../MyLibrary/CsvReader.h"
#include "../Player.h"
#include "../Enemy/Enemy.h"
#include "StageObject.h"

namespace STAGE
{
	const VECTOR3 ADD_POSITION = { -5000.0f, 0.0f, -5000.0f };
	const float CHECK_ONGROUND_LENGTH = 500.0f;
	const int DATA_SIZE = 16;
}

Stage::Stage(int number)
{
	char filename[STAGE::DATA_SIZE];
	sprintf_s<STAGE::DATA_SIZE>(filename, "Stage%02d", number);
	ReadMappingData(filename);
}

Stage::~Stage()
{
}

void Stage::ReadMappingData(std::string filename)
{
	const std::string folder = "data/stage/";
	CsvReader* csv = new CsvReader(folder + filename + ".csv");
	Transform t;
	int hp;
	int score;
	char file[STAGE::DATA_SIZE];

	for (int line = 0; line < csv->GetLines(); line++) {
		int sortNumber = csv->GetInt(line, DATA_NUM::OBJECT);
		
		t.position_.x = csv->GetFloat(line, DATA_NUM::POSITION_X);
		t.position_.y = csv->GetFloat(line, DATA_NUM::POSITION_Y);
		t.position_.z = csv->GetFloat(line, DATA_NUM::POSITION_Z);
		t.position_ += STAGE::ADD_POSITION;
		hp = csv->GetInt(line, DATA_NUM::HP);

		if (sortNumber == OBJECT_SORT::OBJ_PLAYER) {
			new Player(t.position_, hp);
		}
		else
		{
			t.rotation_.x = csv->GetFloat(line, DATA_NUM::ROTATION_X);
			t.rotation_.y = csv->GetFloat(line, DATA_NUM::ROTATION_Y);
			t.rotation_.z = csv->GetFloat(line, DATA_NUM::ROTATION_Z);
			t.scale_.x = csv->GetFloat(line, DATA_NUM::SCALE_X);
			t.scale_.y = csv->GetFloat(line, DATA_NUM::SCALE_Y);
			t.scale_.z = csv->GetFloat(line, DATA_NUM::SCALE_Z);
			score = csv->GetInt(line, DATA_NUM::SCORE);

			if (sortNumber == OBJECT_SORT::OBJ_CHARA) {
				int enemyNumber = csv->GetInt(line, DATA_NUM::NUMBER);
				sprintf_s<STAGE::DATA_SIZE>(file, "enemy%03d", csv->GetInt(line, DATA_NUM::NUMBER));
				Enemy::CreateEnemy(enemyNumber, file, t, hp, score);
			}
			else if (sortNumber == OBJECT_SORT::OBJ_OBJECT) {
				sprintf_s<STAGE::DATA_SIZE>(file, "Stage_Obj%03d", csv->GetInt(line, DATA_NUM::NUMBER));
				StageObject* obj = new StageObject(file, t, hp, score);
			}
		}
	}
	delete csv;
}
