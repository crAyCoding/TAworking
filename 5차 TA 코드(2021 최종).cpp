#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <stdio.h>
using namespace std;

/*/////////////////////////////////////////////////////////////////////////////////////////////////

제작자 : 서건(국방디지털융합 19, cray1124@ajou.ac.kr)
출석 기록이 담긴 파일 이름은 (현재 주차) + (원하는 파일 이름) 으로 변경해주시기 바랍니다!
예시 : 확인할 수업이 8주차라면 출석기록 파일을 "8일본어.txt", 인코딩 : ANSI 로 저장해주시고 
아래 #define의 week와 file_name을 변경해 주시면 됩니다.

추가적으로 질문/요구 사항이 필요한 경우, 상단 이메일로 연락 주시기 바랍니다.

/////////////////////////////////////////////////////////////////////////////////////////////////*/
#define week 14 // 현재 주차 ex) 13주차 출석 기록을 원하는 경우, 13으로 변경해주세요

/*
#define file_name "주차실강채팅_j1B교시.txt" // 출석 기록이 담긴 .txt 파일의 이름을 입력해주세요
#define csv_file_name "1201_"+to_string(week)+"주차_J1B교시.csv"
#define class_start 'B' // 수업 시간을 입력해주세요 . ex) D교시(13:30~14:45) 인 경우, D 입력
#define student_file_name "J1B교시학생목록.txt" // 학생 이름이 담긴 .txt 파일의 이름을 입력해주세요
*/



#define file_name "주차실강채팅_j1D교시.txt" 
#define csv_file_name "1201_"+to_string(week)+"주차_J1D교시.csv"
#define class_start 'D' 
#define student_file_name "J1D교시학생목록.txt" 



//현재 작업상황 : 외국인 구별 불가

map<string, int> student;
map<string, string> student_csv;
map<string, int> final_check;
void push_student(); // 학생 명부 상단 map에 저장 함수
void execute_txt(); // zoom txt 파일 기록 / 출석 시간 분석 함수
void print_txt(); // txt 결과 출력 함수
void check_csv(); // 엑셀 파일 기록 / 분석 함수
void print_csv(); // csv 결과 출력 함수


int main()
{
	push_student();
	execute_txt();
	print_txt();
	check_csv();
	print_csv();
	return 0;
}



void push_student()
{
	string title_s = student_file_name;
	ifstream student_file;
	student_file.open(title_s);
	if (student_file.fail())
	{
		cerr << title_s + " 파일을 찾을 수 없습니다." << endl;
		exit(100);
	}
	string now_student;
	while (!student_file.eof())
	{
		getline(student_file, now_student);
		if (now_student == "") continue;
		if (now_student[0] < 'A' || now_student[0] > 'z')
			student.insert(pair<string, int>(now_student, 0));
	}
	student_file.close();
}

void execute_txt()
{
	string title_t = to_string(week) + file_name;
	ifstream txt_file;
	txt_file.open(title_t);
	if (txt_file.fail())
	{
		cerr << title_t + " 파일을 찾을 수 없습니다." << endl;
		exit(100);
	}
	bool temp = true;
	string nowline;
	while (!txt_file.eof())
	{
		getline(txt_file, nowline);
		if (nowline == "") continue;
		if (temp)
		{
			int name_start = 0, name_end = 0;
			name_start = (nowline.find("발신자")) + 7;
			name_end = (nowline.find("수신자")) - 1;
			if (name_start == 0 || name_end == 0) continue;
			string name_not_editted = nowline.substr(name_start, name_end - name_start);
			string now_name1 = "";
			string now_name2 = "";
			for (char s : name_not_editted)
			{
				if (s != '.' && s != ' ')
				{
					if (s < '0' || s > '9') now_name1 += s;
				}
			}
			int name_size = now_name1.size();
			now_name2 = now_name1.substr(name_size - 2, 2) + now_name1.substr(0, name_size - 2);
			auto name_find1 = student.find(now_name1);
			if (name_find1 != student.end())
			{
				name_find1->second++;
				if ((name_find1->second) / 1000 == 0)
				{
					string now_time = nowline.substr(0, 5);
					now_time.erase(2, 1);
					int time = stoi(now_time);
					name_find1->second += time * 1000;
				}

			}
			else
			{
				auto name_find2 = student.find(now_name2);
				if (name_find2 != student.end())
				{
					name_find2->second++;
					if ((name_find2->second) / 1000 == 0)
					{
						string now_time = nowline.substr(0, 5);
						now_time.erase(2, 1);
						int time = stoi(now_time);
						name_find2->second += time * 1000;
					}
				}
				else
				{
					//student.insert(pair<string, int>(now_name1, 0));
				}
			}
			temp = false;
		}
		else temp = true;

	}
	txt_file.close();
}

void print_txt()
{
	cout << endl << endl << "                     ";
	cout << week << "주차 일본어1 " << class_start << "교시 수업 출석 결과입니다." << endl << endl;
	//cout << "---------------------------------출석--------------------------------------" << endl;
	vector<string> late_person;
	vector<string> absent_person;
	vector<string> full_absent_person;
	vector<string> less_chat_person;
	for (map<string, int>::iterator it = student.begin(); it != student.end(); it++)
	{
		if (it->second != 0)
		{
			int tmp = (((int)class_start) - 65);
			int late_time = 0;
			if (tmp % 2 == 0) late_time = ((it->second) / 1000) - (900 + 300 * tmp);
			else late_time = ((it->second) / 1000) - (900 + ((150 * tmp) - 20));

			int chattt = it->second % 1000;
			string nowname = it->first;

			if (late_time <= 5)
			{
				/*if (nowname.size() == 4)
				{
					nowname = nowname.substr(0, 2) + "  " + nowname.substr(2, 2);
					//cout << nowname;
				}
				else cout << it->first;
				printf(" : 출석 , 채팅 %2d회\n", chattt);*/
			}
			else if (late_time <= 15)
			{
				string now = it->first + " : " + to_string(late_time) + "분지각 "
					+ ", 채팅 " + to_string((it->second) % 1000) + "회";
				late_person.push_back(now);
			}
			else
			{
				string now = it->first + " : " + to_string(late_time) + "분결석 "
					+ ", 채팅 " + to_string((it->second) % 1000) + "회";
				absent_person.push_back(now);
			}
			if (it->second % 1000 < 2)
			{
				string now = it->first + "\t채팅 횟수 부족";
				less_chat_person.push_back(now);
			}
		}
		else
		{
			string now = it->first + " : 채팅 0회 ";
			full_absent_person.push_back(now);
		}
	}
	/*
	if (!late_person.empty())
	{
		cout << "---------------------------------지각--------------------------------------" << endl;
		for (string late : late_person) cout << late << endl;
	}
	if (!absent_person.empty())
	{
		cout << "---------------------------------결석--------------------------------------" << endl;
		for (string abs : absent_person) cout << abs << endl;
	}
	if (!full_absent_person.empty())
	{
		cout << "---------------------------------불참--------------------------------------" << endl;
		for (string full_abs : full_absent_person) cout << full_abs << endl;
	}
	if (!less_chat_person.empty())
	{
		cout << "-------------------------------채팅부족------------------------------------" << endl;
		for (string less : less_chat_person) cout << less << endl;
	}
	*/
	//	cout << "---------------------------------------------------------------------------" << endl;

}

void check_csv()
{
	vector<string> csv_log;
	vector<string> csv_name;
	vector<string> csv_intime;
	vector<string> csv_outtime;
	vector<string> csv_min;
	fstream csv;
	string now_str;
	csv.open(csv_file_name,ios::in);
	int cnt = 0;
	while (!csv.eof())
	{
		cnt++;
		if (cnt % 6 == 0)	getline(csv, now_str);
		else	getline(csv, now_str, ',');
		csv_log.push_back(now_str);
	}
	for (int i = 6; i < csv_log.size(); i++)
	{
		if (i % 6 == 0 && csv_log[i] != "")
		{
			string name_str = csv_log[i];
			if (name_str[0] == ' ') name_str.erase(0, 1);
			for (int k = 0; k < name_str.size(); k++)
			{
				if (name_str[k] == ' ' && name_str.size() == 7)
				{
					name_str = name_str.substr(k + 1, 2) + name_str.substr(0, 4);
				}
			}
			csv_name.push_back(name_str);
		}
		if (i % 6 == 1 && csv_log[i] != "")
		{
			string time_str = csv_log[i];
			vector<int> for_save;
			int num;
			for (int j = 0; j < time_str.size(); j++)
			{
				if (time_str[j] == ':') time_str[j] = ' ';
			}
			stringstream strm;
			strm.str(time_str);
			while (strm >> num)	for_save.push_back(num);
			csv_intime.push_back(to_string(for_save[3] * 100 + for_save[4]));
		}
	}
	for (int i = 0; i < csv_name.size(); i++)
	{
		auto ll = student_csv.find(csv_name[i]);
		if (ll != student_csv.end())
		{
			if (csv_intime[i] < ll->second)
			{
				ll->second = csv_intime[i];
			}
		}
		else student_csv.insert({ csv_name[i],csv_intime[i] });
	}
	csv.close();
}


void print_csv()
{
	cout << endl << endl;
	cout << "                                       결과                                            " << endl;
	cout << " ──────────────────────────────────────────────────────────────────────────────────────" << endl;
	cout << "│ 이  름│   채팅 │ 지각 여부 │ 이  름│   채팅 │ 지각 여부 │ 이  름│   채팅 │ 지각 여부 │" << endl;
	cout << " ──────────────────────────────────────────────────────────────────────────────────────" << endl;
	vector<string> for_print;
	int cnt = 0;


	map<string, int> final_student;
	for (map<string, string>::iterator it = student_csv.begin(); it != student_csv.end(); it++)
	{
		int tmp = (((int)class_start) - 65);
		int nowtime = stoi(it->second);
		nowtime = nowtime / 100 * 60 + nowtime % 100;
		int classtime = 540 + tmp * 90;
		int late_time = nowtime - classtime;
		final_student.insert({ it->first,late_time });
	}
	for (map<string, int>::iterator itt = student.begin(); itt != student.end(); itt++)
	{
		string nowname = itt->first;
		string for_save = "";
		if (nowname[0] >= 'A' && nowname[0] <= 'Z') continue;
		if (nowname.size() == 4)
		{
			nowname = nowname.substr(0, 2) + "  " + nowname.substr(2, 2);
		}
		auto find_student = final_student.find(itt->first);
		if (find_student != final_student.end())
		{
			int late_time = find_student->second;
			int chat_time = (itt->second) % 1000;
			string chat, late;
			if (chat_time < 10)	chat = "   " + to_string(chat_time);
			else chat = "  " + to_string(chat_time);
			if (late_time < 10) late = " " + to_string(late_time);
			else late = to_string(late_time);
			if (find_student->second > 15)
			{
				for_save = " " + nowname + "│ " + chat + "회 │ " + late + "분 결석 │";
			}
			else if (find_student->second > 5)
			{
				for_save = " " + nowname + "│ " + chat + "회 │ " + late + "분 지각 │";
			}
			else
			{
				for_save = " " + nowname + "│ " + chat + "회 │           │";
			}
		}
		else
		{
			for_save = " " + nowname + "│      X │ 전체 결석 │";
		}
		if (cnt >= 20)
		{
			for_print[cnt % 20] += for_save;
		}
		else for_print.push_back("│" + for_save);
		cnt++;
	}

	for (int i = 0; i < 20; i++)
	{
		string now = for_print[i];
		while (now.size() < 80)
		{
			now += "       │        │           │";
		}
		cout << now << endl;
		cout << " ──────────────────────────────────────────────────────────────────────────────────────" << endl;
	}
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	//cout << "---------------------------------------------------------------------------" << endl;
}
