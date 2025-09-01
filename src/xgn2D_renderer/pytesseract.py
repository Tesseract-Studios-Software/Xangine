import renderer

def start(codes, scene, name='PyTesseract', width=800, height=600, tickspeed=30):
    '''codes (list): containing all functions to run
    scene: PyTesseract scene object
    name (str): Window name (Default 'PyTesseract')
    width (int): Window width (Default 800)
    height (int): Window height (Default 600)
    tickspeed (int): EXPERIMENTAL VERSION, no use yet (Default 30)
    
    IMPORTANT:
    All commands in codes must have the following arguments: scene, pressed_keys, fps, firstrun'''

    tick = 0
    loop(scene, tickspeed, codes)

    return scene

# scene: PyTesseract scene object
# canvas: Tkinter canvas object
# limitfps: int
# codes: dict (with functions)
def loop(scene, tickspeed, codes, fps=[0]):
    for code in codes:
        code(scene, fps)

    return renderer.render()