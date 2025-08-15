from PIL import Image, ImageDraw

def create_square_image(sizex, sizey, color, output_path):
    # Create a new image with the given size and white background
    image = Image.new('RGB', (sizex, sizey), '#FF0000')
    draw = ImageDraw.Draw(image)
    
    # Save the image to the specified output path
    image.save(output_path)

# Example usage
create_square_image(64, 64, '', './textures/target.png')