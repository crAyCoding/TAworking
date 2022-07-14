#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
using namespace std;

/*/////////////////////////////////////////////////////////////////////////////////////////////////

제작자 : 서건(국방디지털융합 19, cray1124@ajou.ac.kr)
출석 기록이 담긴 파일 이름은 (현재 주차) + (원하는 파일 이름) 으로 변경해주시기 바랍니다!
예시 : 확인할 수업이 8주차라면 출석기록 파일을 "8일본어.txt", 인코딩 : ANSI 로 저장해주시고 
아래 #define의 week와 file_name을 변경해 주시면 됩니다.

추가적으로 질문/요구 사항이 필요한 경우, 상단 이메일로 연락 주시기 바랍니다.

/////////////////////////////////////////////////////////////////////////////////////////////////*/
#define week 1 // 현재 주차 ex) 13주차 출석 기록을 원하는 경우, 13으로 변경해주세요
#define file_name "aa.txt" // 출석 기록이 담긴 .txt 파일의 이름을 입력해주세요
#define class_start 'B' // 수업 시간을 입력해주세요 . ex) D교시(13:30~14:45) 인 경우, D 입력
#define student_file_name "확인.txt" // 학생 이름이 담긴 .txt 파일의 이름을 입력해주세요

map<string,int> student;

//필요 작업상황 : 엑셀 파일 확인 추가 , 외국인 학생 관리 추가

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
		if(now_student[0] < 'A' || now_student[0] > 'z')	student.insert(pair<string, int>(now_student, 0));
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

void print_all()
{
	vector<string> late_person;
	vector<string> absent_person;
	vector<string> less_chat_person;
	for (map<string,int>::iterator it = student.begin(); it != student.end(); it++)
	{
		if (it->second != 0)
		{
			int tmp = (((int)class_start) - 65);
			int late_time = 0;
			if (tmp % 2 == 0) late_time = ((it->second) / 1000) - (900 + 300 * tmp);
			else late_time = ((it->second) / 1000) - (900 + ((150*tmp)-20)) ;


			if (late_time <= 5)
			{
				cout << it->first << " :  정상출석 " << ", 채팅 횟수 : " << (it->second) % 1000 << endl;
			}
			else if (late_time <= 15)
			{
				string now = it->first + " :  " + to_string(late_time) + "분지각 " + ", 채팅 횟수 : " + to_string((it->second) % 1000);
				late_person.push_back(now);
			}
			else
			{
				string now = it->first + " :  " + to_string(late_time) + "분결석 " + ", 채팅 횟수 : " + to_string((it->second) % 1000);
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
			string now = it->first + " :  전체결석 ";
			absent_person.push_back(now);
		}
	}

	cout << endl;
	for (string late : late_person) cout << late << endl;
	cout << endl;
	for (string abs : absent_person) cout << abs << endl;
	cout << endl;
	for (string less : less_chat_person) cout << less << endl;

}




int main()
{
	push_student();
	execute_txt();
	print_all();
}
