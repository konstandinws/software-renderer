from PIL import Image
frames = [Image.open(f"frames/frame_{i}.tga").convert("RGB") for i in range(36)]
frames[0].save("orbit.gif", save_all=True, append_images=frames[1:], duration=50, loop=0)