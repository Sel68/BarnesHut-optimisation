import matplotlib.pyplot as plt
import matplotlib.animation as animation
import matplotlib.patches as patches
import numpy as np

FILENAME = "simulation_output.txt"
FPS = 30
MIN_BLOB_SIZE = 10
MAX_BLOB_SIZE = 300
ALPHA = 1.0
PADDING = 1.2

def parse_simulation_data(filename):
    frames = []
    current_frame = []
    try:
        with open(filename, 'r') as f:
            for line in f:
                line = line.strip()
                if not line:
                    if current_frame:
                        frames.append(np.array(current_frame))
                        current_frame = []
                    continue
                try:
                    parts = line.split(',')
                    x, y, m = float(parts[0]), float(parts[1]), float(parts[2])
                    if np.isfinite(x) and np.isfinite(y):
                        current_frame.append([x, y, m])
                except ValueError:
                    continue
        if current_frame:
            frames.append(np.array(current_frame))
    except FileNotFoundError:
        print(f"Error: {filename} not found.")
        exit()
    return frames

def update(frame_idx, frames, scatter, rect, ax, title, global_max_mass):
    if frame_idx >= len(frames):
        return scatter, rect, title

    data = frames[frame_idx]
    
    if len(data) > 0:

        scatter.set_offsets(data[:, :2])
        
        masses = data[:, 2]
        
        # handle global mass = 0
        if global_max_mass > 0:
            relative_sizes = (masses / global_max_mass) * MAX_BLOB_SIZE
        else:
            relative_sizes = masses * 0 + MIN_BLOB_SIZE

        # Clamp: [MIN, MAX]
        final_sizes = np.clip(relative_sizes, MIN_BLOB_SIZE, MAX_BLOB_SIZE)
        
        scatter.set_sizes(final_sizes)
        
        # Red Box
        max_coord = np.max(np.abs(data[:, :2]))
        half_dim = max_coord * 1.5 + 10.0
        
        rect.set_xy((-half_dim, -half_dim))
        rect.set_width(half_dim * 2)
        rect.set_height(half_dim * 2)
        
        view_limit = half_dim * PADDING
        ax.set_xlim(-view_limit, view_limit)
        ax.set_ylim(-view_limit, view_limit)

    title.set_text(f"Step: {frame_idx}")
    return scatter, rect, title

def main():
    frames = parse_simulation_data(FILENAME)
    if not frames: 
        print("No data found.")
        return

    print(f"Loaded {len(frames)} frames.")

    all_masses = [p[2] for frame in frames for p in frame]
    global_max_mass = max(all_masses) if all_masses else 1.0
    print(f"Global Max Mass: {global_max_mass}")

    fig, ax = plt.subplots(figsize=(10, 10), facecolor='black')
    ax.set_facecolor('black')
    ax.set_aspect('equal')
    ax.grid(False)

    rect = patches.Rectangle((0,0), 1, 1, linewidth=2, edgecolor='red', facecolor='none', linestyle='--')
    ax.add_patch(rect)

    scatter = ax.scatter([], [], s=MIN_BLOB_SIZE, c='cyan', alpha=ALPHA, edgecolors='white', linewidth=0.5)
    title = ax.text(0.05, 0.95, "", transform=ax.transAxes, color="white", fontsize=12)

    ani = animation.FuncAnimation(
        fig, update, frames=len(frames), 
        fargs=(frames, scatter, rect, ax, title, global_max_mass),
        interval=1000/FPS, blit=False
    )
    ani.save("simulation.gif", writer="pillow", fps=FPS)

if __name__ == "__main__":
    main()