import curses
import threading
import time
import random
import math
from collections import defaultdict
from itertools import cycle
from tabulate import tabulate


def read_file(filename):
    with open(filename, encoding='utf-8') as f:
        return [line.strip().split() for line in f.readlines()]


examiners = read_file("examiners.txt")
students = read_file("students.txt")
questions = [line.strip().split() for line in open("questions.txt", encoding="utf-8")]

GOLDEN_RATIO = (1 + math.sqrt(5)) / 2

student_queue = students.copy()
exam_results = {}
examiner_stats = {ex[0]: {"total": 0, "failed": 0, "time": 0, "current_student": "-"} for ex in examiners}
question_stats = defaultdict(int)

lock = threading.Lock()
exam_start_time = time.time()


def choose_answer(words, gender):
    probabilities = [1 / GOLDEN_RATIO]
    for _ in range(len(words) - 1):
        probabilities.append((1 - sum(probabilities)) / GOLDEN_RATIO)
    return random.choices(words, probabilities if gender == "М" else probabilities[::-1])[0]


def examiner_process(name):
    global student_queue
    name_length = len(name)

    while True:
        with lock:
            if not student_queue:
                break
            student = student_queue.pop(0)

        student_name, gender = student
        examiner_stats[name]["current_student"] = student_name
        examiner_stats[name]["total"] += 1

        correct_answers = 0
        total_answers = 0

        for question in random.sample(questions, 3):
            student_answer = choose_answer(question, gender)
            examiner_answers = []
            while len(examiner_answers) < len(question) and random.random() < 1 / 3:
                examiner_answers.append(random.choice(question))
            examiner_answers = list(set(examiner_answers))
            correct = student_answer in examiner_answers
            correct_answers += correct
            total_answers += 1
            if correct:
                question_stats[" ".join(question)] += 1

        mood = random.choices(["bad", "neutral", "good"], [1 / 8, 5 / 8, 1 / 4])[0]

        if mood == "bad" or (mood == "neutral" and correct_answers <= total_answers / 2):
            exam_results[student_name] = "Провалил(а)"
            examiner_stats[name]["failed"] += 1
        else:
            exam_results[student_name] = "Сдал(а)"

        exam_duration = random.uniform(name_length - 1, name_length + 1)
        examiner_stats[name]["time"] += exam_duration
        time.sleep(exam_duration)
        examiner_stats[name]["current_student"] = "-"

        if time.time() - exam_start_time >= 30:
            time.sleep(random.uniform(12, 18))


def display_status(stdscr):
    curses.curs_set(0)
    curses.start_color()

    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK)   # Успех
    curses.init_pair(2, curses.COLOR_RED, curses.COLOR_BLACK)     # Провал
    curses.init_pair(3, curses.COLOR_YELLOW, curses.COLOR_BLACK)  # В ожидании
    curses.init_pair(4, curses.COLOR_CYAN, curses.COLOR_BLACK)    # Заголовки
    curses.init_pair(5, curses.COLOR_WHITE, curses.COLOR_BLACK)   # Обычный текст

    while any(thread.is_alive() for thread in threads):
        stdscr.clear()
        student_status = {s: exam_results.get(s, "Очередь") for s, _ in students}
        sorted_students = sorted(student_status.items(), key=lambda x: (x[1] != "Очередь", x[1] == "Провалил(а)"))

        remaining_count = len([s for s in student_status.values() if s == "Очередь"])
        total_students = len(students)
        elapsed_time = round(time.time() - exam_start_time)

        max_y, max_x = stdscr.getmaxyx()
        y = 0

        stdscr.attron(curses.color_pair(4) | curses.A_BOLD)
        stdscr.addstr(y, 0, "СТАТУС СТУДЕНТОВ")
        stdscr.attroff(curses.color_pair(4) | curses.A_BOLD)
        y += 1

        for name, status in sorted_students:
            color = 1 if status == "Сдал(а)" else 2 if status == "Провалил(а)" else 3
            stdscr.attron(curses.color_pair(color))
            stdscr.addstr(y, 0, f"{name:<20} {status}")
            stdscr.attroff(curses.color_pair(color))
            y += 1
            if y >= max_y - 1:
                stdscr.addstr(y, 0, "Недостаточно места на экране", curses.color_pair(2) | curses.A_BOLD)
                break

        if y < max_y - 1:
            y += 1
            stdscr.attron(curses.color_pair(4) | curses.A_BOLD)
            stdscr.addstr(y, 0, "СТАТУС ЭКЗАМЕНАТОРОВ")
            stdscr.attroff(curses.color_pair(4) | curses.A_BOLD)
            y += 1
            for e, s in examiner_stats.items():
                if y >= max_y - 1: break
                stdscr.addstr(y, 0,
                              f"{e:<15} | Текущий: {s['current_student']:<15} | Всего: {s['total']:<3} | "
                              f"Завалил: {s['failed']:<3} | Время: {s['time']:.2f} сек.")
                y += 1

        if y < max_y - 2:
            y += 1
            stdscr.attron(curses.color_pair(5))
            stdscr.addstr(y, 0, f"Осталось в очереди: {remaining_count} из {total_students}")
            y += 1
            stdscr.addstr(y, 0, f"Время с начала экзамена: {elapsed_time} сек.")
            stdscr.attroff(curses.color_pair(5))

        stdscr.refresh()
        time.sleep(1)

    stdscr.clear()
    stdscr.addstr(0, 0, "Экзамен завершён. Нажмите любую клавишу...", curses.color_pair(4) | curses.A_BOLD)
    stdscr.refresh()
    stdscr.getch()

def colored_print(text, color_code=37, bold=False):
    bold_code = '1;' if bold else ''
    return f"\033[{bold_code}{color_code}m{text}\033[0m"

# Определяем цветовые коды, соответствующие curses-парам
COLORS = {
    1: 32,  # GREEN (Сдал)
    2: 31,  # RED (Провал)
    3: 33,  # YELLOW (Данные)
    4: 36,  # CYAN (Заголовки)
    5: 37   # WHITE (Обычный текст)
}


# Запускаем потоки
threads = []
for examiner in examiners:
    thread = threading.Thread(target=examiner_process, args=(examiner[0],))
    thread.start()
    threads.append(thread)

curses.wrapper(display_status)

for thread in threads:
    thread.join()


# Находим процент заваленных студентов для каждого экзаменатора
examiner_failure_rates = {
    e: s["failed"] / s["total"] if s["total"] > 0 else 1  # Если экзаменатор не принимал студентов, считаем 100% заваленных
    for e, s in examiner_stats.items()
}

# Находим минимальный процент заваленных студентов

min_failure_rate = min(examiner_failure_rates.values())

# Финальный вывод
exam_duration = time.time() - exam_start_time
best_students = [s for s, r in exam_results.items() if r == "Сдал(а)"]
best_examiners = [
    e for e, rate in examiner_failure_rates.items() if rate == min_failure_rate
]
failing_students = [s for s, r in exam_results.items() if r == "Провалил(а)"]
best_questions = [q for q, count in sorted(question_stats.items(), key=lambda x: -x[1])[:2]]
exam_success = sum(1 for r in exam_results.values() if r == "Сдал(а)") / len(exam_results) > 0.85

print("\n" + colored_print("Результаты экзамена", COLORS[4], bold=True))
print(tabulate(
    [
        [
            colored_print(s, COLORS[1] if r == "Сдал(а)" else COLORS[2]),
            colored_print(r, COLORS[1] if r == "Сдал(а)" else COLORS[2])
        ]
        for s, r in sorted(exam_results.items(), key=lambda x: x[1] == "Сдал(а)", reverse=True)
    ],
    headers=[colored_print("Студент", COLORS[4]), colored_print("Статус", COLORS[4])]
))
print()
print(tabulate(
    [
        [
            colored_print(e, COLORS[5]),
            colored_print(str(s["total"]), COLORS[5]),
            colored_print(str(s["failed"]), COLORS[2]),
            colored_print(f"{round(s['time'], 2)}", COLORS[3])
        ]
        for e, s in examiner_stats.items()
    ],
    headers=[
        colored_print("Экзаменатор", COLORS[4]),
        colored_print("Всего студентов", COLORS[4]),
        colored_print("Завалил(а)", COLORS[4]),
        colored_print("Время работы", COLORS[4])
    ]
))

print(colored_print(f"\nВремя с начала экзамена: {round(exam_duration, 2)} сек.", COLORS[5]))
print(colored_print(f"Лучшие студенты: {', '.join(best_students)}", COLORS[1]))
print(colored_print(f"Лучшие экзаменаторы: {', '.join(best_examiners)}", COLORS[3]))
print(colored_print(f"Отчисленные студенты: {', '.join(failing_students)}", COLORS[2]))
print(colored_print(f"Лучшие вопросы: {', '.join(best_questions)}", COLORS[3]))
print(colored_print(f"Экзамен {'удался' if exam_success else 'не удался'}",
                   COLORS[1] if exam_success else COLORS[2], bold=True))
