#########################################
# 프로그램명: 성적관리프로그램
# 작성자: 컴퓨터공학과 / 홍길동
# 작성일: 2025-04-11
# 프로그램 설명: 5명의 학생 정보를 입력받아 성적을 계산 및 관리하는 프로그램
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


class GradeManagement:
    def __init__(self):
        self.students = []

    def input_student(self):
        number = input("학번 입력: ")
        name = input("이름 입력: ")
        english = int(input("영어 점수 입력: "))
        c = int(input("C언어 점수 입력: "))
        python = int(input("파이썬 점수 입력: "))
        student = Student(number, name, english, c, python)
        student.calculate_total_and_average()
        student.calculate_grade()
        self.students.append(student)

    def calculate_ranks(self):
        self.students.sort(key=lambda x: x.total, reverse=True)
        for idx, student in enumerate(self.students):
            student.rank = idx + 1

    def delete_student(self, number):
        self.students = [s for s in self.students if s.number != number]

    def search_student(self, key, value):
        if key == 'number':
            return [s for s in self.students if s.number == value]
        elif key == 'name':
            return [s for s in self.students if s.name == value]

    def sort_by_total(self):
        self.students.sort(key=lambda x: x.total, reverse=True)

    def count_above_80(self):
        return len([s for s in self.students if s.average >= 80])

    def display_students(self):
        print(f"{'학번':<10}{'이름':<10}{'영어':<6}{'C언어':<6}{'파이썬':<6}{'총점':<6}{'평균':<6}{'학점':<6}{'등수':<6}")
        for student in self.students:
            print(f"{student.number:<10}{student.name:<10}{student.english:<6}{student.c:<6}{student.python:<6}{student.total:<6}{student.average:<6.2f}{student.grade:<6}{student.rank:<6}")


# 프로그램 실행
if __name__ == "__main__":
    gm = GradeManagement()
    while True:
        print("\n1. 학생 입력")
        print("2. 학생 삭제")
        print("3. 학생 검색")
        print("4. 총점 기준 정렬")
        print("5. 80점 이상 학생 수")
        print("6. 학생 출력")
        print("7. 종료")
        choice = int(input("메뉴 선택: "))

        if choice == 1:
            gm.input_student()
        elif choice == 2:
            number = input("삭제할 학생의 학번 입력: ")
            gm.delete_student(number)
        elif choice == 3:
            key = input("검색 기준 (number/name): ")
            value = input("검색 값 입력: ")
            results = gm.search_student(key, value)
            for student in results:
                print(f"학번: {student.number}, 이름: {student.name}, 총점: {student.total}, 평균: {student.average:.2f}, 학점: {student.grade}, 등수: {student.rank}")
        elif choice == 4:
            gm.sort_by_total()
            print("총점 기준으로 정렬 완료.")
        elif choice == 5:
            count = gm.count_above_80()
            print(f"80점 이상 학생 수: {count}")
        elif choice == 6:
            gm.calculate_ranks()
            gm.display_students()
        elif choice == 7:
            print("프로그램 종료.")
            break
        else:
            print("잘못된 입력입니다. 다시 선택하세요.")