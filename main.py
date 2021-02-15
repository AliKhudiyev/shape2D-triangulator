import numpy as np
import os
from tkinter import *

root = Tk()
root.title('2D Shape Triangulator')

canvas = Canvas(root, width=640, height=640, bg = '#afeeee')
frame = Frame(root)

canvas.pack()
frame.pack()

type_label = Label(frame, text='')
type_label.grid(row=0, column=2)

radius = 2
points = list()
triangles = list()


def draw_triangle(triangle):
    for i, point in enumerate(triangle):
        canvas.create_line(point[0], point[1], 
                           triangle[(i+1)%len(triangle)][0], triangle[(i+1)%len(triangle)][1],
                           fill='green', dash=(3,3))

def refresh():
    canvas.delete('all')
    for i, point in enumerate(points):
        canvas.create_oval(point[0]-radius, point[1]-radius, point[0]+radius, point[1]+radius, fill='black')
        canvas.create_line(point[0], point[1], points[(i+1)%len(points)][0], points[(i+1)%len(points)][1])
    for triangle in triangles:
        draw_triangle(triangle)
    root.after(250, refresh)

def reset():
    global triangles
    triangles = list()

def clear():
    global points
    reset()
    points = list()

def create_point(event):
    global points
    points.append([event.x, event.y])
    refresh()

def remove_point(event):
    global points
    for i in range(len(points)):
        if np.sqrt((points[i][0]-event.x)**2 + (points[i][1]-event.y)**2) <= 3*radius:
            points.remove(points[i])
            break
    refresh()

def triangluate_shape():
    global points, triangles
    global type_label

    command = 'build/triangulate shape.csv > triangles.txt'

    with open('shape.csv', 'w') as f:
        for point in points:
            f.write(f'{point[0]}, {point[1]}\n')
    f.close()

    os.system(command)

    with open('triangles.txt', 'r') as f:
        line = f.readline()
        # print(line[:-1])
        indices = line[:-1].split(' ')[:-1]
        # type_ = lines[-1][5:-1]
        # print(indices)
        for i in range(0, len(indices), 3):
            triangles.append([points[int(indices[i])], 
                              points[int(indices[i+1])], 
                              points[int(indices[i+2])]])
        
        # type_label.config(text=type_)
    f.close()

def generate_shape():
    global points, triangles
    global type_var, nVertex_entry

    command = 'build/main -g '
    n_vertex = None
    points = list()
    triangles = list()
    
    if type_var.get():
        command += 'cv '
    else:
        command += 'cx '
    command += '-n '
    try:
        n_vertex = int(nVertex_entry.get())
        command += str(n_vertex)
        command += ' > shape.csv'
        os.system(command)
    except:
        print('Invalid number of vertices!')
    
    with open('shape.csv', 'r') as f:
        lines = f.readlines()[:n_vertex]
        for line in lines:
            coords = [320+50*float(coord) for coord in line[1:-2].split(',')]
            points.append(coords)
    f.close()


canvas.bind('<Button-1>', create_point)
canvas.bind('<Button-2>', remove_point)

type_var = IntVar(value=0)
Radiobutton(frame, text='Convex', variable=type_var, value=0).grid(row=1, column=1)
Radiobutton(frame, text='Concave', variable=type_var, value=1).grid(row=1, column=2)

nVertex_entry = Entry(frame)
nVertex_entry.grid(row=1, column=3)

Button(frame, text='Triangulate', command=triangluate_shape).grid(row=0, column=0)
Button(frame, text='Generate', command=generate_shape).grid(row=1, column=0)
Button(frame, text='Reset', command=reset).grid(row=0, column=1)
Button(frame, text='Clear', command=clear).grid(row=0, column=2)

root.after(250, refresh)
root.mainloop()
