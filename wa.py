eng=[0]*5
c=[0]*5
py=[0]*5
sumi=[0]*5
rank=[0]*5
number=[0]*10
name=[0]*5
hak=[0]*5

def scann(x):
    number[x]=input(f'{i+1}번째 학생의 학번을 입력하시오: ')
    name[x]=input(f'{i+1}번째 학생의 이름을 입력하시오: ')
    eng[x] = float(input(f'{i+1}번째 학생의 영어 점수를 입력하시오: '))
    c[x] = float(input(f'{i+1}번째 학생의 c언어 점수를 입력하시오: '))
    py[x] = float(input(f'{i+1}번째 학생의 파이썬 점수를 입력하시오: '))
    return 

def sum_calcul(x):
    sumi[x]=eng[x]+c[x]+py[x]
    return

def hak_calcul(x):
    if sumi[x]>=270: hak[x]='A+'
    elif sumi[x]>=255: hak[x]='A'
    elif sumi[x]>=240: hak[x]='B+'
    elif sumi[x]>=225: hak[x]='B'
    elif sumi[x]>=210: hak[x]='C+'
    elif sumi[x]>=195: hak[x]='C'
    else: hak[x]='D'
    return

def rank_calcul(): 
    for i in range(5):
        cnt=1
        for j in range(5):
             if sumi[i]<sumi[j]: 
                cnt+=1
        rank[i]=cnt
    return 



for i in range(5):
    scann(i)
    sum_calcul(i)
    hak_calcul(i)
    print('\n')

rank_calcul()

print("성적관리 프로그램\n")
print(" =============================================================================\n")
print("학번       이름   영어   C-언어   파이썬   총점  평균  학점  등수 \n")
print(" =============================================================================\n")


for i in range(5):
    print(number[i],name[i],int(eng[i]),int(c[i]),int(py[i]),int(sumi[i]),int(sumi[i]/3),hak[i],rank[i])
