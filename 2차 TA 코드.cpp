#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

#define num_week 13       // 현재 주차
#define now_class false  // B교시수업은 true , D교시 수업은 false

int main(void)
{
	ifstream check_file; // 채팅기록파일
	ifstream student_file; // 학생명단파일
	map<string, int> student; // 학생명단 map 생성, string : 학생 이름, int : 출석 횟수
	string file_name, now_student;
	int limit_time = 1030;

	if (now_class)
	{
		file_name = to_string(num_week) + "주차실강채팅_j1B교시.txt";
		student_file.open("J1B교시학생목록.txt");
		cout << to_string(num_week) + "주차 일본어1 B교시 수업 출석 기록입니다!" << endl << endl;
	}
	else
	{
		file_name = to_string(num_week) + "주차실강채팅_j1D교시.txt";
		student_file.open("J1D교시학생목록.txt");
		cout << to_string(num_week) + "주차 일본어1 D교시 수업 출석 기록입니다!" << endl << endl;
		limit_time += 300;
	}

	/////////////////////////학생 명부 배열 저장///////////////////////// 

	if (student_file.fail()) // 파일 열기 실패
	{
		cerr << "FILE NOT FOUND" << endl;
		exit(100);
	}


	while (!student_file.eof())
	{
		getline(student_file, now_student);
		if (now_student == "") continue;
		student.insert(pair<string,int>(now_student, 0));
	}
	student_file.close();


	/////////////////////////채팅 기록 파일 열람///////////////////////// 

	int num_of_student = (int)student.size();
	int* chat_time = (int*)calloc(num_of_student, sizeof(int));

	check_file.open(file_name);

	if (check_file.fail()) // 파일 열기 실패
	{
		cerr << "FILE NOT FOUND" << endl;
		exit(100);
	}

	cout << endl << "------------------------------지각여부------------------------------" << endl << endl;

	int line = 1;
	string nowline,prevline;

	while (!check_file.eof())
	{
		getline(check_file, nowline);
		int number = 0;
		if (line % 2 == 1) // 홀수줄, 시간, 줌 설정 이름 존재
		{
			prevline = nowline;
			for (auto iter = student.begin(); iter != student.end(); iter++)
			{
				if (nowline.find(iter->first) != string::npos)
				{
					if (chat_time[number] == 0)
					{
						string now_time = nowline.substr(0,2) + nowline.substr(3,2);
						int final_time = stoi(now_time) - limit_time;
						if (final_time == 0) chat_time[number] = -1;
						else
						{
							chat_time[number] = final_time;
							if (final_time > 3 && final_time <= 15)
							{
								cout << iter->first + " : 지각입니다, 늦은 시간 : " + to_string(final_time) + "분" << endl;
							}
							else if (final_time > 15 && final_time < 30)
							{
								cout << iter->first + " : 결석입니다, 늦은 시간 : " + to_string(final_time) + "분" << endl;
							}
							else if (final_time >= 30)
							{
								cout << iter->first + " : 결석입니다, 늦은 시간 : " + to_string(final_time-40) + "분" << endl;
								chat_time[number] = final_time - 40;
							}
						}
					}
					iter->second++;
					break;
				}
				number++;
			}
		}
		else
		{
			for (auto iter = student.begin(); iter != student.end(); iter++)
			{
				if (nowline.find(iter->first) != string::npos && chat_time[number] == 0)
				{
					string now_time = prevline.substr(0, 2) + prevline.substr(3, 2);
					int final_time = stoi(now_time) - limit_time;
					if (final_time == 0) chat_time[number] = -1;
					else
					{
						chat_time[number] = final_time;
						if (final_time > 5 && final_time <= 15)
						{
							cout << iter->first + " : 지각입니다, 늦은 시간 : " + to_string(final_time) + "분" << endl;
						}
						else if (final_time > 15)
						{
							cout << iter->first + " : 결석입니다, 늦은 시간 : " + to_string(final_time) + "분" << endl;
						}
					}
				}
				number++;
			}
		}
		line++;
	}

	cout << endl  << "------------------------------채팅기록 2회 이하------------------------------" << endl << endl;

	
	int number = 0;

	for (auto iter = student.begin(); iter != student.end(); iter++)
	{
		if (chat_time[number++] == 0)
		{
			cout << iter->first << "\t: 채팅기록 없음,결석" << endl;
		}
		else
		{
			if (iter->second == 0) cout << iter->first << "\t: 채팅기록 없음(줌 이름과 실명 비동일)" << endl;
			else if(iter->second <= 2) cout << iter->first << "\t: 채팅기록 부실(2회 이하)" << endl;
		}
	}

	cout << endl << "------------------------------정상출석자 채팅횟수------------------------------" << endl << endl;

	number = 0;
	for (auto iter = student.begin(); iter != student.end(); iter++)
	{
		if (iter->second > 2)
		{
			int num = iter->second;
			cout << iter->first << "\t: ";
			printf("%2d\n", num);
		}
		number++;
	}

}
