# Parallel Image Processing with OpenMP

This project compares sequential and OpenMP implementations of Gaussian blur
and Sobel edge detection.

## How to run

1. Install Visual Studio with the **Desktop development with C++** workload.
2. Open Visual Studio.
3. Select **File > Open > Folder**.
4. Open the `parallel-image-processing-openmp` folder.
5. Wait for CMake to finish configuring the project. The first setup can take
   several minutes because it installs OpenCV.
6. From **Select Startup Item**, choose `image_processor.exe`.
7. Click the green Run button.

If `image_processor.exe` does not appear, select
**Project > Delete Cache and Reconfigure**, then wait for CMake to finish.

## Output

The program saves the processed images and benchmark results in:

```text
images/output
```

To generate the performance chart, open Developer PowerShell in Visual Studio
and run:

```powershell
python scripts\plot_results.py images\output\benchmark_results.csv --output images\output\performance.png
```

The chart will be saved as `images/output/performance.png`.
