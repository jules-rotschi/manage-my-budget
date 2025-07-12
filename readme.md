# Manage My Budget

## Description

Manage My Budget is an application that allows users to sort their bank operations by category (users create their own categories) and to see reviews so they can optimize the usage of their money.

## Windows deployment

### Generate Ninja file

<pre>cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -B build</pre>

### Build with Ninja

<pre>ninja -C build</pre>

### Install application on system

<pre>cmake --install build</pre>
The installation directory is in "Program Files (x86)".
(Needs to be ran as administrator)