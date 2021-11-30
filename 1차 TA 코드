#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define num_week 7       // 현재 주차
#define now_class false  // B교시수업은 true , D교시 수업은 false

int main(void)
{
	ifstream check_file;
	
	string j1B = to_string(num_week) + "주차실강채팅_j1B교시.txt";
	string j1D = to_string(num_week) + "주차실강채팅_j1D교시.txt";


	/////////////////////////학생 명부 배열 저장///////////////////////// 

	ifstream student_file;
	vector<string> student;
	string now_student;

	cout << to_string(num_week);

	if (now_class)
	{
		student_file.open("J1B교시학생목록.txt");
		cout << "주차 일본어1 B교시 수업 출석 기록입니다!" << endl << endl;
	}
	else
	{
		student_file.open("J1D교시학생목록.txt");
		cout << "주차 일본어1 D교시 수업 출석 기록입니다!" << endl << endl;
	}

	while (!student_file.eof())
	{
		getline(student_file, now_student);
		if (now_student == "")
		{
			break;
		}
		else
		{
			student.push_back(now_student);
		}
	}
	student_file.close();


	/////////////////////////채팅 기록 파일 열람///////////////////////// 


	if (now_class)
	{
		check_file.open(j1B);
	}
	else
	{
		check_file.open(j1D);
	}
	bool seq = true;
	string first_line; // 홀수 줄, 시간기록과 줌 자체 닉네임 나오는 줄
	string second_line; // 짝수 줄, 채팅기록
	string now_time; // 홀수 줄에서 시간만 따로 추출 -> 이후 int형으로 바꿔줄 예정.

	vector<string> person;
	vector<string> late_person;
	vector<string> absent_person;


	if (check_file.fail()) // 파일 열기 실패
	{
		cerr << "FILE NOT FOUND" << endl;
		exit(100);
	}

	cout << "-----------------------지각자 명단-----------------------" << endl;

	while (!check_file.eof())
	{
		
		if (seq) //첫번째 줄 읽는 구간, 시간 및 줌 설정 이름 표시됨
		{
			first_line.clear();
			now_time.clear();
			getline(check_file, first_line);
			remove(first_line.begin(), first_line.end(), ':');
			now_time = first_line.substr(0, 4); // 00:00 형식으로 되어 있는 줌 내용을 0000의 숫자만 추출하여 문자열에 저장
			seq = false;
		}
		else //두번째 줄 읽는 구간 , 채팅 내용 표시됨
		{
			second_line.clear();
			getline(check_file, second_line);
			string name = second_line.substr(1, second_line.length() - 1); // 채팅기록 추출
			int time = stoi(now_time);

			if (first_line.find(name) != string::npos && name.size()>=3) // 채팅기록과 닉네임이 일치하는 경우
			{
				if (now_class)
				{

					if (time > 1035)
					{
						late_person.push_back(name); // 10:30이 지난 시간, 지각처리
						cout << name + " : 1(지각) , " + to_string(time-1030) << "분 늦음" << endl;
					}
					else if (time > 1045) // 10:45이 지난 시간, 결석처리
					{
						absent_person.push_back(name);
						cout << name + " : 2(결석) , " + to_string(time-1030) << "분 늦음" << endl;
					}
					else
					{
						person.push_back(name);
					}
				}
				else
				{
					if (time > 1335)
					{
						late_person.push_back(name); // 13:30이 지난 시간, 지각처리
						cout << name + " : 1(지각) , " + to_string(time - 1330) << "분 늦음" << endl;
					}
					else if (time > 1345) // 13:45이 지난 시간, 결석처리
					{
						absent_person.push_back(name);
						cout << name + " : 2(결석) , " + to_string(time - 1330) << "분 늦음" << endl;
					}
					else
					{
						person.push_back(name);
					}
				}
			}
			seq = true;
		}
	}

	/////////////////////////가나다순 정렬///////////////////////// 


	sort(late_person.begin(), late_person.end());
	sort(person.begin(), person.end());
	sort(absent_person.begin(), absent_person.end());

	vector<string> normal_presence;
	vector<string> abnormal_presence;
	vector<string> presence;


	check_file.close();

	/////////////////////////정상출석, 비정상출석 구분///////////////////////// 



	for (string str : person)
	{
		ifstream file;
		string log;
		if (now_class)
		{
			file.open(j1B);
		}
		else
		{
			file.open(j1D);
		}
		int count = 0;
		bool seq = true;
		while (!file.eof())
		{
			string nowline;
			if (seq)
			{
				getline(file, nowline);
				if (nowline.find(str) != string::npos)
				{
					count++;
				}
				seq = false;
			}
			else
			{
				getline(file, nowline);
				if (nowline.find(str) != string::npos)
				{
					count++;
				}
				seq = true;
			}
		}
		if (count > 3)
		{
			log = str + "\t출석, 채팅횟수 : " + to_string(count);
			normal_presence.push_back(log);
		}
		else if (count == 3)
		{
			log = str + "\t출석, 채팅기록 이름 , '출'만 존재";
			abnormal_presence.push_back(log);
		}
		else
		{
			log = str + "\t출석, 채팅기록 이름만 존재";
			abnormal_presence.push_back(log);
		}
		presence.push_back(str);
		file.close();
	}
	for (string str : late_person)
	{
		ifstream file;
		string log;
		if (now_class)
		{
			file.open(j1B);
		}
		else
		{
			file.open(j1D);
		}
		int count = 0;
		bool seq = true;
		while (!file.eof())
		{
			string nowline;
			if (seq)
			{
				getline(file, nowline);
				if (nowline.find(str) != string::npos)
				{
					count++;
				}
				seq = false;
			}
			else
			{
				getline(file, nowline);
				if (nowline.find(str) != string::npos)
				{
					count++;
				}
				seq = true;
			}
		}
		if (count > 3)
		{
			log = str + "\t지각, 채팅횟수 :" + to_string(count);
			abnormal_presence.push_back(log);
		}
		else if (count == 3)
		{
			log = str + "\t지각, 채팅기록 이름,'출'만 존재";
			abnormal_presence.push_back(log);
		}
		else
		{
			log = str + "\t지각, 채팅기록 이름만 존재";
			abnormal_presence.push_back(log);
		}
		presence.push_back(str);
		file.close();
	}
	for (string str : absent_person)
	{
		ifstream file;
		string log;
		if (now_class)
		{
			file.open(j1B);
		}
		else
		{
			file.open(j1D);
		}
		int count = 0;
		bool seq = true;
		while (!file.eof())
		{
			string nowline;
			if (seq)
			{
				getline(file, nowline);
				if (nowline.find(str) != string::npos)
				{
					count++;
				}
				seq = false;
			}
			else
			{
				getline(file, nowline);
				if (nowline.find(str) != string::npos)
				{
					count++;
				}
				seq = true;
			}
		}
		if (count > 3)
		{
			log = str + "\t결석(시간늦음), 채팅횟수 :" + to_string(count);
			abnormal_presence.push_back(log);
		}
		else if (count == 3)
		{
			log = str + "\t결석(시간늦음), 채팅기록 이름 , '출'만 존재";
			abnormal_presence.push_back(log);
		}
		else
		{
			log = str + "\t결석(시간늦음), 채팅기록 이름만 존재";
			abnormal_presence.push_back(log);
		}
		presence.push_back(str);
		file.close();
	}

	for (string a : person)
	{
		for (int i = 0; i < student.size(); i++)
		{
			if (a == student[i]) student[i] = "";
		}
	}
	for (string a : late_person)
	{
		for (int i = 0; i < student.size(); i++)
		{
			if (a == student[i]) student[i] = "";
		}
	}
	for (string a : absent_person)
	{
		for (int i = 0; i < student.size(); i++)
		{
			if (a == student[i]) student[i] = "";
		}
	}

	/////////////////////////실행 내용 출력///////////////////////// 


	cout << "-----------------------정상출석자-----------------------" << endl << endl;
	for (string a : normal_presence)
	{
		cout << a << endl;
	}

	cout << endl << endl << "-----------------------지각/결석/채팅기록 위반-----------------------" << endl << endl;

	for (string a : abnormal_presence)
	{
		cout << a << endl;
	}

	cout << endl << endl << "-----------------------추가 확인 필요 학생-----------------------" << endl << endl;

	for (string a : student)
	{
		if (a != "")
		{
			cout << a << "\t본인 이름 적은 채팅 기록 없음, 추가 확인 필요!" << endl;
		}
	}

}
