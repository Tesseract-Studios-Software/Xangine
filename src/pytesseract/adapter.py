import tkinter as tk
import pyautogui # type: ignore
from PIL import Image, ImageTk # type: ignore

# This module combines many modules to provide a simple interface for other parts of this program

def start(name, width, height):
    root = tk.Tk()
    root.title(name)
    root.geometry(f'{width}x{height}')
    canvas = tk.Canvas(root, width=width, height=height, bg='black')
    canvas.pack()
    return root, canvas, width, height

def bindkey(canvas, key, action):
    def wrapper(event):
        action(event)
    canvas.bind(key, wrapper)

def display(data, canvas, width, height):
    photo = ImageTk.PhotoImage(data)
    canvas.create_image(width // 2, height // 2, image=photo)
    # Keep a reference to the image to prevent it from being garbage collected
    canvas.image = photo

def mainloop():
    tk.mainloop()