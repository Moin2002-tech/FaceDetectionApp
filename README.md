# FaceDetectionApp
Face Detection Project: Feature_Detection1  C++ implementation of face detection using machine learning &amp; computer vision techniques. Detects faces in images &amp; videos for security, surveillance &amp; human-computer interaction.  Key Features  Face detection using SVMs &amp; Probabilistic Classifiers Face tracking using Condensation algorithm.


# FaceDetectionApp Installation Guide

# Requirements
Windows 10 or Windows 11

Visual Studio Community 2022 C++

VCPKG (Package Manager)

# Required External Libraries
opencv:x64-windows (dynamic)

boost:x64-windows (dynamic)

matio:x64-windows (dynamic, for MATLAB manipulation)

highfive::x64-windows (dynamic, hdf5 file manipulation)

# VCPKG Installation Process
Open a terminal (PowerShell or Command Prompt).

Clone the vcpkg repository from GitHub:

sh
git clone https://github.com/microsoft/vcpkg.git
Navigate to the vcpkg directory:

sh
cd vcpkg
#Build vcpkg:

sh
.\bootstrap-vcpkg.bat

# Integrate vcpkg with Visual Studio:
sh
.\vcpkg integrate install
This command sets up user-wide integration, making it easier to use vcpkg with Visual Studio projects.

#Install Packages:

sh
.\vcpkg install [package-name]
Replace [package-name] with the name of the package you want to install.

#Install External Libraries
Run the following commands to install the required external libraries:

sh
.\vcpkg install boost:x64-windows
.\vcpkg install opencv:x64-windows
.\vcpkg install matio:x64-windows
.\vcpkg install highfive:x64-windows
Final Integration
After installing the libraries, run this command:

sh
.\vcpkg integrate project
With a project open, go to Tools -> NuGet Package Manager -> Package Manager Console and paste:

sh
```Install-Package "vcpkg.D.dev.vcpkg" -Source```
To find the NuGet Package Manager in Visual Studio:

Open your project in Visual Studio.

Go to the top menu and select Tools.

From the dropdown, select NuGet Package Manager.

Then, click on Package Manager Console.

Copy the quoted command and paste it into the NuGet Package Manager Console in Visual Studio. You're now ready to run all files.

# Prebuild binaries of External libraries
 this prebuild Binaries Just Extract it and integrate it but it does not have ``matio``,``highfive`` so to install! use above commands
https://16sxj-my.sharepoint.com/:u:/g/personal/shaikh_moin12_16sxj_onmicrosoft_com/EbROfwKln7FCrL3lAoEqHSoBx-EJ3a7nVgRBL-bjrgy6kA?e=SMpjwE
