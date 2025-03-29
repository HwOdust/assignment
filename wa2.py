import os
import time
import random
print("틱텍토 게임!!")
p=3
game=[[0 for i in range(p)] for i in range(p)]
print("배치하고 싶은 좌표입력(EX 3 2)")
def victory(x,y,o):
    global game
    global p
    m=[]
    l=0
    
    for i in range(p):
        if game[i][p-i-1]==o:

            l+=1
        else:
            break
    m.append(l)
    for i in range(p):
        if game[i][i]==o:
            l+=1
        else:
            break
    m.append(l)
    for i in range(p):
        l=0
        for j in range(p):
            if game[i][j]==o:
                l+=1
            else:
                break
        m.append(l)
    for i in range(p):
        l=0
        for j in range(p):
            if game[j][i]==o:
                l+=1
            else:
                break
        m.append(l)
    time.sleep(1)
    if p in m:
        return True
    else:
        return False
def find():
    global game
    m=[]
    for i in range(p):
        for j in range(p):
            if game[i][j]==0:
                m.append([i,j])
    return m
def draw():
    for i in range(p):
        print("==="*p)
        for j in range(p):
            if game[i][j]==0:
                print("ㅒ ",end="")
            elif game[i][j]==1:
                print("ㅒO",end="")
            else:
                print("ㅒX",end="")
        print("ㅒ")
    print("==="*p)
round=0
def main():
    global p
    global round
    global game
    pl=random.randrange(1,3)
    while True:
        
        round+=1
        draw()
        while True:
            if round%2==pl-1:
                print(("O " if round%2==1 else "X ")+"입력하세요(종료: -1 -1)")
            else:
                print("컴퓨터 입력")
            try:
                x=0
                y=0
                if round%2==pl-1:
                    x,y=map(int,input().split())
                else:
                    k=find()
                    k=k[random.randrange(0,len(k))]
                    x=k[1]
                    y=k[0]
                    print(x,y)
                game[y-1][x-1]=1 if round%2==1 else 2
                if victory(x,y,1 if round%2==1 else 2 ):
                    os.system("cls")
                    draw()
                    print(("O " if round%2==1 else "X ")+"승리")
                    return
                if x==-1 and y==-1:
                    return
                if game[y-1][x-1]!=1 and game[y-1][x-1]!=2:
                    continue
                break
            except:
                continue
        os.system("cls")
main()