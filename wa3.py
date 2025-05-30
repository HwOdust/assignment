import sqlite3
import sys
import io
sys.stdout = io.TextIOWrapper(sys.stdout.detach(), encoding='utf-8')
DB_PATH="C:/SQLite/grades.db"

#########################################
# 프로그램명: 성적관리프로그램
# 작성자: 소프트웨어학부부 / 최희원
# 기능: 학생 정보를 DB에 저장하고 성적을 계산하여 관리하는 프로그램
#########################################


class Student:
    def __init__(self, number, name, english, c, python):
        self.number = number
        self.name = name
        self.english = english
        self.c = c
        self.python = python
        self.total = 0
        self.average = 0.0
        self.grade = ''
        self.rank = 0

    def calculate_total_and_average(self):
        self.total = self.english + self.c + self.python
        self.average = self.total / 3

    def calculate_grade(self):
        if self.average >= 90:
            self.grade = 'A'
        elif self.average >= 80:
            self.grade = 'B'
        elif self.average >= 70:
            self.grade = 'C'
        elif self.average >= 60:
            self.grade = 'D'
        else:
            self.grade = 'F'


class GradeDatabase:
    def __init__(self):
        self.db_name = DB_PATH
        self.create_table()

    def connect(self):
        return sqlite3.connect(self.db_name)

    def create_table(self):
        conn = self.connect()
        cur = conn.cursor()
        cur.execute("""
            CREATE TABLE IF NOT EXISTS students (
                number TEXT PRIMARY KEY,
                name TEXT,
                english INTEGER,
                c INTEGER,
                python INTEGER,
                total INTEGER,
                average REAL,
                grade TEXT,
                rank INTEGER
            )
        """)
        conn.commit()
        conn.close()

    def insert_student(self, student):
        conn = self.connect()
        cur = conn.cursor()
        cur.execute("""
            INSERT INTO students VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)
        """, (student.number, student.name, student.english, student.c, student.python,
              student.total, student.average, student.grade, student.rank))
        conn.commit()
        conn.close()

    def delete_student(self, number):
        conn = self.connect()
        cur = conn.cursor()
        cur.execute("DELETE FROM students WHERE number = ?", (number,))
        conn.commit()
        conn.close()

    def search_student(self, key, value):
        conn = self.connect()
        cur = conn.cursor()
        if key == 'number':
            cur.execute("SELECT * FROM students WHERE number = ?", (value,))
        elif key == 'name':
            cur.execute("SELECT * FROM students WHERE name = ?", (value,))
        result = cur.fetchall()
        conn.close()
        return result

    def sort_by_total(self):
        conn = self.connect()
        cur = conn.cursor()
        cur.execute("SELECT * FROM students ORDER BY total DESC")
        result = cur.fetchall()
        conn.close()
        return result

    def count_above_80(self):
        conn = self.connect()
        cur = conn.cursor()
        cur.execute("SELECT COUNT(*) FROM students WHERE average >= 80")
        result = cur.fetchone()[0]
        conn.close()
        return result

    def update_ranks(self):
        students = self.sort_by_total()
        conn = self.connect()
        cur = conn.cursor()
        for idx, s in enumerate(students):
            cur.execute("UPDATE students SET rank = ? WHERE number = ?", (idx + 1, s[0]))
        conn.commit()
        conn.close()

    def fetch_all(self):
        conn = self.connect()
        cur = conn.cursor()
        cur.execute("SELECT * FROM students ORDER BY total DESC")
        result = cur.fetchall()
        conn.close()
        return result


# 프로그램 실행
def main():
    db = GradeDatabase()

    while True:
        print("\n1. 학생 입력")
        print("2. 학생 삭제")
        print("3. 학생 검색")
        print("4. 총점 기준 정렬")
        print("5. 80점 이상 학생 수")
        print("6. 학생 출력")
        print("7. 종료")

        choice = input("메뉴 선택: ")

        if choice == '1':
            number = input("학번 입력: ")
            name = input("이름 입력: ")
            english = int(input("영어 점수 입력: "))
            c = int(input("C언어 점수 입력: "))
            python = int(input("파이썬 점수 입력: "))
            student = Student(number, name, english, c, python)
            student.calculate_total_and_average()
            student.calculate_grade()
            db.insert_student(student)
            db.update_ranks()
            print("학생 정보가 저장되었습니다.")

        elif choice == '2':
            number = input("삭제할 학생의 학번 입력: ")
            db.delete_student(number)
            db.update_ranks()
            print("학생 정보가 삭제되었습니다.")

        elif choice == '3':
            key = input("검색 기준 (number/name): ")
            value = input("검색 값 입력: ")
            results = db.search_student(key, value)
            for s in results:
                print(f"학번: {s[0]}, 이름: {s[1]}, 총점: {s[5]}, 평균: {s[6]:.2f}, 학점: {s[7]}, 등수: {s[8]}")

        elif choice == '4':
            sorted_students = db.sort_by_total()
            for s in sorted_students:
                print(f"학번: {s[0]}, 이름: {s[1]}, 총점: {s[5]}")

        elif choice == '5':
            count = db.count_above_80()
            print(f"80점 이상 학생 수: {count}")

        elif choice == '6':
            db.update_ranks()
            students = db.fetch_all()
            print(f"{'학번':<10}{'이름':<10}{'영어':<6}{'C언어':<6}{'파이썬':<6}{'총점':<6}{'평균':<6}{'학점':<6}{'등수':<6}")
            for s in students:
                print(f"{s[0]:<10}{s[1]:<10}{s[2]:<6}{s[3]:<6}{s[4]:<6}{s[5]:<6}{s[6]:<6.2f}{s[7]:<6}{s[8]:<6}")

        elif choice == '7':
            print("프로그램 종료.")
            break

        else:
            print("잘못된 입력입니다. 다시 선택하세요.")


if __name__ == "__main__":
    main()
