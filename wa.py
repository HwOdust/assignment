eng=[0]*5
c=[0]*5
py=[0]*5
sumi=[0]*5
rank=[0]*5
number=[0]*10
name=[0]*5
hak=[0]*5


for i in range(5):
    number[i]=input(f'{i+1}번째 학생의 학번을 입력하시오: ')
    name[i]=input(f'{i+1}번째 학생의 이름을 입력하시오: ')
    eng[i] = float(input(f'{i+1}번째 학생의 영어 점수를 입력하시오: '))
    c[i] = float(input(f'{i+1}번째 학생의 c언어 점수를 입력하시오: '))
    py[i] = float(input(f'{i+1}번째 학생의 파이썬 점수를 입력하시오: '))
    sumi[i]=eng[i]+c[i]+py[i]
    if sumi[i]>=270: hak[i]='A+'
    elif sumi[i]>=255: hak[i]='A'
    elif sumi[i]>=240: hak[i]='B+'
    elif sumi[i]>=225: hak[i]='B'
    elif sumi[i]>=210: hak[i]='C+'
    elif sumi[i]>=195: hak[i]='C'
    else: hak[i]='D'

for i in range(5):
    cnt=1
    for j in range(5):
        if sumi[i]<=sumi[j]: 
            cnt+=1
    rank[i]=cnt


print("성적관리 프로그램\n")
print(" =============================================================================\n")
print("학번       이름   영어   C-언어   파이썬   총점  평균  학점  등수 \n")
print(" =============================================================================\n")


for i in range(5):
    print(number[i],name[i],int(eng[i]),int(c[i]),int(py[i]),int(sumi[i]),int(sumi[i]/3),hak[i],rank[i],"\n")