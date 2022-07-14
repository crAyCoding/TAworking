from tabnanny import check
import tkinter.ttk as ttk
import os
from tkinter import *
from tkinter import filedialog

cray = Tk()
cray.title("Zoom Attendance")
cray.geometry('1080x800')
cray.resizable(False,False)

#주차, 수업시간 설정 프레임
start_frame = LabelFrame(cray,text = '현재 주차 / 수업 시간 선택',labelanchor='n')
start_frame.pack(fill='both')

week_label = ttk.Label(start_frame,text = "현재 주차 :")
week_label.pack(side = "left",padx = 130,pady = 15)

week_arr = []
for i in range (1,17) :
    week_arr.append(str(i) + '주')
week_cmb = ttk.Combobox(start_frame,state = 'readonly',values=week_arr)
week_cmb.current(0)
week_cmb.pack(side = "left")

class_label = ttk.Label(start_frame,text = "수업 시간 :")
class_label.pack(side = "left",padx = 130,pady = 15)

class_arr = []
for i in range (0,6) :
    class_arr.append(str(chr(ord('A') + i)) + "교시")
class_cmb = ttk.Combobox(start_frame,state = 'normal',values = class_arr)
class_cmb.current(0)
class_cmb.pack(side = 'left')

#프로그램 사진
photo = PhotoImage(file = "c:/Users/USER/Desktop/python_practice/Zoom.png")
label1 = Label(cray,image = photo)
label1.pack()

#엑셀 파일 선택 프레임
excel_file_frame = LabelFrame(cray,text="Zoom 관련 Excel 파일 추가",labelanchor="n")
excel_file_frame.pack(fill = "both")

def excel_file_add() :
    new_excel_file = filedialog.askopenfilenames(title = '찾아보기', \
        filetypes=(("모든 파일","*.*"),(".csv","*.csv*"),(".xlsx","*.*"),(".cell","*.cell*")), initialdir="c:/")
    
    for file in new_excel_file :
        excel_file_list.insert(END,file)

def excel_file_del() :
    for clicked in reversed(excel_file_list.curselection()) :
        excel_file_list.delete(clicked)

excel_file_list = Listbox(excel_file_frame,selectmode = "extended",width = 40,height = 1)
excel_file_list.pack(side="left",fill="both",expand=True)

excel_file_addbtn = Button(excel_file_frame, text = "엑셀 파일 추가",width = 15,height = 1,command = excel_file_add)
excel_file_addbtn.pack(fill="both")

excel_file_delbtn = Button(excel_file_frame, text = "엑셀 파일 제거",width = 15,height = 1,command = excel_file_del)
excel_file_delbtn.pack(fill="both")

#기능 선택 프레임
second_frame = LabelFrame(cray,text = "기능 선택",labelanchor="n")
second_frame.pack(fill = "both")

### 출 / 결 확인 ###

def check_absent() :
    student_absent = {}
    default_time = 540 + (ord(class_cmb.get()[0]) - ord('A')) * 90
    absent_file = open(excel_file_list.get(0),'r')
    is_first_line = True
    while True :
        now_line = absent_file.readline()
        if not now_line :
            break
        if(is_first_line) :
            is_first_line = False
            continue
        splited_string = now_line.split(',')
        out_time_array = splited_string[2].split(' ')
        student_name = splited_string[0]
        if student_name in student_absent :
            continue
        in_time = (int)(splited_string[1][-5:-3]) * 60 + (int)(splited_string[1][-2:])
        student_absent[student_name] = in_time
        out_time = out_time_array[1]
    for student in student_absent.keys() :
        if(student_absent[student] > default_time) :
            print(student)

    absent_file.close()

check_btn = Button(second_frame,text = "출 / 결\n확인",width = 30,height = 3,command = check_absent)
check_btn.pack(side = "left")

### 채팅기록 확인 ###

chatlog_btn = Button(second_frame,text = "채팅기록\n확인",width = 30,height = 3)
chatlog_btn.pack(side="left")

### 채팅 그래프 확인 ###

graph_btn = Button(second_frame,text = "채팅 그래프\n확인",width = 30,height = 3)
graph_btn.pack(side="left")

### 결과 다운로드 ###

making_file_btn = Button(second_frame,text = "결과\n다운로드",width = 30,height = 3)
making_file_btn.pack(side="left")


#마지막 프레임
cray_frame = LabelFrame(cray,text = '뀨',labelanchor='se')
cray_frame.pack(fill='both')

finish_btn = Button(cray_frame,text='프로그램 종료',width = 12,height = 1)
finish_btn.pack(side = 'right')

cray.mainloop()

