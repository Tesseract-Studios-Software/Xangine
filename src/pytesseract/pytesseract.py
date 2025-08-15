import renderer
import objects
import scenes
import adapter
import time
import os
import multitasking

def start(codes, scene, name='PyTesseract', width=800, height=600, tickspeed=30):
    '''codes (list): containing all functions to run
    scene: PyTesseract scene object
    name (str): Window name (Default 'PyTesseract')
    width (int): Window width (Default 800)
    height (int): Window height (Default 600)
    tickspeed (int): EXPERIMENTAL VERSION, no use yet (Default 30)
    
    IMPORTANT:
    All commands in codes must have the following arguments: scene, pressed_keys, fps, firstrun'''
    
    data = adapter.start(name, width, height)
    root = data[0]
    canvas = data[1]
    width = data[2]
    height = data[3]

    scene.canvas = canvas

    pressed_keys = set()

    def key_press(event):
        pressed_keys.add(event.keysym)

    def key_release(event):
        pressed_keys.discard(event.keysym)

    canvas.bind_all('<KeyPress>', key_press)
    canvas.bind_all('<KeyRelease>', key_release)
    canvas.bind('<Button-1>', lambda event: objects.sensing.on_click)

    tick = 0
    loop(scene, canvas, tickspeed, codes, pressed_keys)
    adapter.mainloop()

    return scene

def loop(scene, canvas, tickspeed, codes, pressed_keys, fps=[0], firstrun=True):
    # scene: PyTesseract scene object
    # canvas: Tkinter canvas object
    # limitfps: int
    # codes: dict (with functions)

    timestart = time.time()

    renderer.display(
        renderer.render(scene),
        canvas,
        800,
        600
    )

    for code in codes:
        @multitasking.task
        def task():
            code(scene, pressed_keys, fps)
        task()

    timeend = time.time()
    fps.append(1 / (timeend - timestart))

    if firstrun:
        @multitasking.task
        def init():
            codes[0](scene, pressed_keys, fps)
        init()

    canvas.after(10, loop, scene, canvas, tickspeed, codes, pressed_keys, fps, False)

def mouse_coords(scene, canvas):
    return scene.bind_mouse_events(canvas)