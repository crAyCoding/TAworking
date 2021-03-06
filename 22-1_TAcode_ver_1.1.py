import tkinter.ttk as ttk
import tkinter.font as tkfont
from tkinter import *
from tkinter import filedialog
from tkinter import messagebox
from turtle import goto

now_class = 2
## 1C교시 : 1, 1E교시 : 2, 2D교시 : 3, 3B교시 : 4

##
students = {}


screen = Tk()
screen.title('출 결 관 리')
screen.geometry('600x800')

#현재 수업 선택 프레임
class_frame = LabelFrame(screen,text = '수업 선택',labelanchor='n')
class_frame.pack(fill='both')

class_selectbox = ttk.Combobox(class_frame,height = 5,values = ['일본어1C','일본어1E','일본어2D','일본어3C'],state = 'readonly')
class_selectbox.current(0)
class_selectbox.pack()


#엑셀 파일 선택 프레임
excel_file_frame = LabelFrame(screen,text="Zoom 출결 Excel 파일 추가",labelanchor="n")
excel_file_frame.pack(fill = "both")
def excel_file_upload() :
    try :
        new_excel_file = filedialog.askopenfilename(title = '찾아보기', \
        filetypes=(("모든 파일","*.*"),(".csv","*.csv*"),(".xlsx","*.*"),(".cell","*.cell*")), \
            initialdir="C:/Users/USER/Desktop/TAfiles")
        if new_excel_file:
            excel_file_list.delete(0,'end')
            excel_file_list.insert(0,new_excel_file)
    except : 
        messagebox.showerror("Error","파일을 선택해 주십시오")

excel_file_list = Listbox(excel_file_frame,selectmode = "extended",width = 40,height = 1)
excel_file_list.pack(side="left",fill="both",expand=True)

excel_file_uploadbtn = Button(excel_file_frame, text = "엑셀 파일 추가",width = 15,height = 1,command = excel_file_upload)
excel_file_uploadbtn.pack(fill="both")

#채팅 파일 선택 프레임
chat_file_frame = LabelFrame(screen,text="Zoom 채팅 txt 파일 추가",labelanchor="n")
chat_file_frame.pack(fill = "both")
def chat_file_upload() :
    try :
        new_chat_file = filedialog.askopenfilename(title = '찾아보기', \
        filetypes=(("모든 파일","*.*"),(".csv","*.csv*"),(".xlsx","*.*"),(".cell","*.cell*")), \
            initialdir="C:/Users/USER/Desktop/TAfiles")
        if new_chat_file:
            chat_file_list.delete(0,'end')
            chat_file_list.insert(0,new_chat_file)
    except : 
        messagebox.showerror("Error","파일을 선택해 주십시오")

chat_file_list = Listbox(chat_file_frame,selectmode = "extended",width = 40,height = 1)
chat_file_list.pack(side="left",fill="both",expand=True)

chat_file_uploadbtn = Button(chat_file_frame, text = "txt 파일 추가",width = 15,height = 1,command = chat_file_upload)
chat_file_uploadbtn.pack(fill="both")


#출결 관리 only
def check_absent() :
    result_text.delete("1.0",END)
    now_class = class_selectbox.get()
    if now_class == '일본어1C':
        std_list = open('C:/Users/USER/Desktop/TAfiles/일본어1C교시학생.txt','r',encoding='utf-8')
    elif now_class == '일본어1E' :
        std_list = open('C:/Users/USER/Desktop/TAfiles/일본어1E교시학생.txt','r',encoding='utf-8')
    elif now_class == '일본어2D' :
        std_list = open('C:/Users/USER/Desktop/TAfiles/일본어2D교시학생.txt','r',encoding='utf-8')
    elif now_class == '일본어3C' :
        std_list = open('C:/Users/USER/Desktop/TAfiles/일본어3B교시학생.txt','r',encoding='utf-8')
    else :
        messagebox.showerror("?","코드에 장난질 하지마라")

    while True :
        now_line = std_list.readline().strip()
        if not now_line :
            break
        students[now_line] = 0
    
    
    try :
        excel_file = open(excel_file_list.get(0),'r')
    except :
        messagebox.showerror("ERROR","엑셀 파일을 넣어 주십시오")
        return
    
    class_time = 0
    is_first_line = True

    while True :
        now_line = excel_file.readline().strip()
        if not now_line :
            break
        if is_first_line :
            is_first_line = False
            continue
        line_split = now_line.split(',')
        
        if line_split[2] == '' :
            continue
        excel_name = line_split[0]
        intime_split = line_split[2].split(' ')
        intime = int(intime_split[1][:2]) * 60 + int(intime_split[1][3:5]) 
        outtime_split = line_split[3].split(' ')
        outtime = int(intime_split[1][:2]) * 60 + int(intime_split[1][3:5]) 
        if outtime > class_time :
            class_time = outtime

        if excel_name == '이 경숙' or excel_name == '이경숙(아주대' or excel_name[:3] == '이경숙' :
            continue

        if excel_name[0] == '.' :
            excel_name = excel_name[1:]
        if excel_name[-1:] == '.' :
            excel_name = excel_name[:-1]
        if excel_name[1] == ' ' :
            excel_name = excel_name[0] + excel_name[2:]

    
        #######################외국인 처리 ##################
        if excel_name[:3] == " JI" :
            excel_name = 'JIN, YINUO(김이낙)'
        if excel_name[:3] == 'SUN' :
            excel_name = 'SUN, JUNJIE(손준걸)'
        if excel_name == 'YAN YUXIN ' or excel_name[:3] == '옌위신':
            excel_name = 'YAN, YUXIN(옌위신)'
        if excel_name == 'GUO TONG ' or excel_name == '곽동':
            excel_name = 'GUO, TONG(곽동)'
        if excel_name[:2] == '보황' :
            excel_name = 'VO, HOANG LONG'
        if excel_name == 'NGUYEN THI NHAT THUONG' :
            excel_name = 'NGUYEN, THI NHAT THUONG'
        if excel_name == 'NGUYEN CHI CUONG ' :
            excel_name = 'NGUYEN, CHI CUONG'
        if excel_name[:3] == 'xuz' or excel_name[:5] == 'XU ZH':
            excel_name = 'XU, ZHUOTAO'
        if excel_name[:10] == 'NGUYEN YEN' :
            excel_name = 'NGUYEN, YEN NHI'
        ######################################################

        if excel_name in students  and students[excel_name] == 0 :
            students[excel_name] = intime
        elif excel_name not in students :
            result_text.insert(END,excel_name+'학생을 찾을 수 없습니다.\n')
        else :
            continue
    result_text.insert(END,'------------------------------------------------------------------------------------\n')

    class_time = int(class_time / 90) * 90
    # 6:A, 7:B, 8:C, 9:D...

    for student in students :
        if students[student] == 0 :
            result_text.insert(END,student+'는 결석입니다.\n')

    result_text.insert(END,'------------------------------------------------------------------------------------\n')
    for student in students :
        if students[student] - class_time > 15 : 
            result_text.insert(END,student + '는 ' + str(students[student] - class_time) + '분 결석입니다.\n')
        elif students[student] - class_time > 5 : 
            result_text.insert(END,student + '는 ' + str(students[student] - class_time) + '분 지각입니다.\n')
        


#출결 통합
def check_absent_chat() :
    result_text.delete("1.0",END)
    now_class = class_selectbox.get()
    for_chat_students = {}
    middle_absent = {}
    students_time = {}
    if now_class == '일본어1C':
        std_list = open('C:/Users/USER/Desktop/TAfiles/일본어1C교시학생.txt','r',encoding='utf-8')
    elif now_class == '일본어1E' :
        std_list = open('C:/Users/USER/Desktop/TAfiles/일본어1E교시학생.txt','r',encoding='utf-8')
    elif now_class == '일본어2D' :
        std_list = open('C:/Users/USER/Desktop/TAfiles/일본어2D교시학생.txt','r',encoding='utf-8')
    elif now_class == '일본어3C' :
        std_list = open('C:/Users/USER/Desktop/TAfiles/일본어3B교시학생.txt','r',encoding='utf-8')
    else :
        messagebox.showerror("?","코드에 장난질 하지마라")

    while True :
        now_line = std_list.readline().strip()
        if not now_line :
            break
        students[now_line] = 0
        middle_absent[now_line] = 0
        students_time[now_line] = 0
    
    
    try :
        excel_file = open(excel_file_list.get(0),'r')
    except :
        messagebox.showerror("ERROR","엑셀 파일을 넣어 주십시오")
        return 

    try :
        chat_file = open(chat_file_list.get(0),'r',encoding = 'utf-8')
    except :
        messagebox.showerror("ERROR","채팅 파일을 넣어 주십시오")
        return 
    
    class_time = 0
    is_first_line = True
    

    result_text.insert(END,'--------------------------------------오류처리--------------------------------------\n')

    while True :
        now_line = excel_file.readline().strip()
        if not now_line :
            break
        if is_first_line :
            is_first_line = False
            continue
        line_split = now_line.split(',')

        if line_split[2] == '' :
            continue
        
        excel_name = line_split[0]
        for_chat_students[excel_name] = 0
        intime_split = line_split[2].split(' ')
        intime = int(intime_split[1][:2]) * 60 + int(intime_split[1][3:5]) 
        outtime_split = line_split[3].split(' ')
        outtime = int(intime_split[1][:2]) * 60 + int(intime_split[1][3:5]) 
        if outtime > class_time :
            class_time = outtime

        if excel_name == '이 경숙' or excel_name == '이경숙(아주대' or excel_name[:3] == '이경숙' :
            continue

        if excel_name[0] == '.' :
            excel_name = excel_name[1:]
        if excel_name[-1:] == '.' :
            excel_name = excel_name[:-1]
        if excel_name[1] == ' ' :
            excel_name = excel_name[0] + excel_name[2:]

    
        #######################외국인 처리 ##################
        if excel_name[:3] == " JI" :
            excel_name = 'JIN, YINUO(김이낙)'
        if excel_name[:3] == 'SUN' :
            excel_name = 'SUN, JUNJIE(손준걸)'
        if excel_name[:3] == 'YAN'  or excel_name[:3] == '옌위신':
            excel_name = 'YAN, YUXIN(옌위신)'
        if excel_name[:3] == 'GUO' or excel_name == '곽동':
            excel_name = 'GUO, TONG(곽동)'
        if excel_name[:2] == '보황' :
            excel_name = 'VO, HOANG LONG'
        if excel_name == 'NGUYEN THI NHAT THUONG' :
            excel_name = 'NGUYEN, THI NHAT THUONG'
        if excel_name == 'NGUYEN CHI CUONG ' :
            excel_name = 'NGUYEN, CHI CUONG'
        if excel_name[:3] == 'xuz' or excel_name[:5] == 'XU ZH':
            excel_name = 'XU, ZHUOTAO'
        if excel_name[:10] == 'NGUYEN YEN' :
            excel_name = 'NGUYEN, YEN NHI'
        ######################################################

        if excel_name in students_time  and students_time[excel_name] == 0 :
            students_time[excel_name] = intime
        elif excel_name not in students :
            result_text.insert(END,excel_name+'학생을 찾을 수 없습니다.\n')
        else :
            continue

    #############################################################################################################################

    
    count_middle = 0
    flag = False
    while True :
        now_line = chat_file.readline().strip()
        if not now_line :
            break
        if flag :
            if now_line.find('중출') == 0 :
                count_middle += 1
                middle_absent[chat_name] = 1
            flag = False
            continue
        else :
            line_split = now_line.split(' ')
            if len(line_split) <= 2 :
                continue
            name_start = 13
            name_end = now_line.find('수신자')
            chat_name = now_line[name_start:name_end-1]
            if len(chat_name) < 2 :
                print(chat_name)
                continue
            if chat_name[0] == '.' :
                chat_name = chat_name[1:]
            if chat_name[:3] == " JI" :
                    chat_name = 'JIN, YINUO(김이낙)'
            elif chat_name[:3] == 'SUN' or chat_name[:3] == '손준걸' :
                chat_name = 'SUN, JUNJIE(손준걸)'
            elif chat_name[:3] == 'YAN'  or chat_name[:3] == '옌위신':
                chat_name = 'YAN, YUXIN(옌위신)'
            elif chat_name[:3] == 'GUO' or chat_name == '곽동':
                chat_name = 'GUO, TONG(곽동)'
            elif chat_name[:3] == '보 황' :
                chat_name = 'VO, HOANG LONG'
            elif chat_name[:10] == 'NGUYEN THI' :
                chat_name = 'NGUYEN, THI NHAT THUONG'
            elif chat_name[:10] == 'NGUYEN CHI' :
                chat_name = 'NGUYEN, CHI CUONG'
            elif chat_name[:3] == 'xuz' or chat_name[:5] == 'XU ZH':
                chat_name = 'XU, ZHUOTAO'
            elif chat_name[:10] == 'NGUYEN YEN' :
                chat_name = 'NGUYEN, YEN NHI'   
            elif len(chat_name) > 5 :
                chat_name = chat_name[:3]
            if chat_name in students :
                students[chat_name] += 1
            else :
                print(chat_name)
            flag = True
    
    
    result_text.insert(END,'--------------------------------------결석확인--------------------------------------\n')

    class_time = int(class_time / 90) * 90
    # 6:A, 7:B, 8:C, 9:D...

    for student in students :
        if students[student] == 0 :
            result_text.insert(END,student+'는 결석입니다.\n')
            middle_absent[student] = 1

    result_text.insert(END,'--------------------------------------지각확인--------------------------------------\n')
    for student in students :
        if students_time[student] - class_time > 15 : 
            result_text.insert(END,student + '는 ' + str(students_time[student] - class_time) + '분 결석입니다.\n')
        elif students_time[student] - class_time > 5 : 
            result_text.insert(END,student + '는 ' + str(students_time[student] - class_time) + '분 지각입니다.\n')
        elif middle_absent[student] == 0 :
            result_text.insert(END,student + '는 중출 지각입니다' + '\n')

    result_text.insert(END,'--------------------------------------채팅확인--------------------------------------\n')


    for student in students :
        result_text.insert(END,student + ' : ' + str(students[student]) + '\n')

    
    for student in students :
        result_text.insert(END,str(students[student]) + '\n')

        





checking_frame = LabelFrame(screen,text='기 능 선 택',labelanchor='n')
checking_frame.pack()

absent_only_check_btn = Button(checking_frame,text='출석만\n확인하기',width = 15,height = 2,command = check_absent)
absent_only_check_btn.pack()

absent_check_btn = Button(checking_frame,text='모두 다\n확인하기',width = 15,height = 2,command = check_absent_chat)
absent_check_btn.pack()


result_frame = LabelFrame(screen,text = "결과",labelanchor='n')
result_frame.pack(fill='both')

result_text = Text(result_frame,width = 150,height = 100)
result_text.pack()

screen.mainloop()
