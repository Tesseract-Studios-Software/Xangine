import sys
import os

# Ensure the local pytesseract module is imported
sys.path.insert(0, os.path.abspath(os.path.dirname(__file__)))

testmode = 1

if testmode == 0:
    import random
    import pytesseract

    scene = None

    scene = pytesseract.scenes.scene(
        {},
        'scene.png',
        {
            'square.png': './textures/square.png',
            'scene.png': './textures/scene.png'
        }
    )

    # Now create the object and add it to the scene
    scene.objects['Square'] = pytesseract.objects.object(500, 500, 0, scene, 'square.png')

    def code(scene, pressed_keys, fps, firstrun):
        if scene.objects['Square'].x == 0:
            scene.objects['Square'].movement.move((1000, 1000), 5)
        else:
            scene.objects['Square'].movement.move((0, 0), 0)
        # scene.objects['Square'].movement.move((random.randint(0, 100), random.randint(0, 100)), 2)

    pytesseract.start(
        [code], 
        scene,
        name='PyTesseract Test',
        width=800,
        height=600,
        tickspeed=30
    )

if testmode == 1:
    from PIL import Image # type: ignore
    import random
    import pytesseract
    import os
    import time
    # import multitasking

    # Initialize the scene
    scene = pytesseract.scenes.scene(
        {},
        'scene.png',
        {
            'player.png': './textures/player.png',
            'target.png': './textures/target.png',
            'scene.png': './textures/scene.png'
        }
    )

    # Create the player and target objects
    player = pytesseract.objects.object(50, 50, 0, scene, 'player.png')
    target = pytesseract.objects.object(random.randint(20, 980), random.randint(20, 980), 0, scene, 'target.png')

    # Add objects to the scene
    scene.objects['Player'] = player
    scene.objects['Target'] = target

    # Initialize score
    score = 0
    timetimer = time.time()  # Initialize timetimer outside the code function
    y = 0
    displayfps = 'No data yet'

    player = scene.objects['Player']
    target = scene.objects['Target']


    def init(scene, pressed_keys, fps):
        global timetimer, y, displayfps, score
        timetimer = time.time()
        y = 0
        displayfps = 'No data yet'

    def main(scene, pressed_keys, fps):
        global timetimer, y, displayfps, score

        # Clear the canvas text
        scene.canvas.delete('fps_text')

        # Check if player reaches the target
        if player.sensing.touching(target):
            score += 1
            # Move target to a new random position
            target.movement.move((random.randint(20, 980), random.randint(20, 980)), 0)

        # Handle multiple key presses for movement
        if 'w' in pressed_keys:
            player.movement.move((player.x, player.y - 500 / fps[-1]), 0)
        if 's' in pressed_keys:
            player.movement.move((player.x, player.y + 500 / fps[-1]), 0)
        if 'a' in pressed_keys:
            player.movement.move((player.x - 500 / fps[-1], player.y), 0)
        if 'd' in pressed_keys:
            player.movement.move((player.x + 500 / fps[-1], player.y), 0)

    def fps(scene, pressed_keys, fps):
        global timetimer, y, displayfps, score
        y += 1
        if time.time() - timetimer >= 1:
            displayfps = f'FPS: {y}'
            timetimer = time.time()
            y = 0

    # Start PyTesseract
    pytesseract.start(
        [init, main, fps],  # Remove individual key press functions and use main for handling movement
        scene,
        name='PyTesseract Test',
        width=800,
        height=600,
        tickspeed=30,
    )