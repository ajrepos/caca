# ============================================
# CHULETA DE COMANDOS DE GRUB
# ============================================

# -------------------- COMPROBACIONES --------------------
# Ver versión de GRUB
grub-install --version

# Verificar sintaxis del archivo de configuración
sudo grub-script-check /boot/grub/grub.cfg

# Ver entradas del menú (nombres exactos de los SO)
sudo grep menuentry /boot/grub/grub.cfg

# Ver particiones del disco
lsblk

# Actualizar GRUB (aplica cambios)
sudo update-grub

# -------------------- CONFIGURACIÓN (editar /etc/default/grub) --------------------
sudo nano /etc/default/grub

# Líneas importantes dentro del archivo:
GRUB_DEFAULT="Windows 10 (on /dev/sda1)"   # SO por defecto
GRUB_TIMEOUT=10                             # Tiempo de espera en segundos
GRUB_TIMEOUT_STYLE=hidden                   # hidden = menú oculto, menu = visible
GRUB_BACKGROUND="/boot/grub/imagen.jpg"     # Imagen de fondo

# Guardar cambios y actualizar
sudo update-grub

# -------------------- RESPALDO --------------------
# Hacer copia de seguridad del archivo de configuración
sudo cp /etc/default/grub /etc/default/grub.backup

# -------------------- IMAGEN DE FONDO --------------------
# Descargar imagen (ejemplo)
wget -O grub_wallpaper.jpg "URL_DE_LA_IMAGEN"

# Copiar a /boot/grub/
sudo cp grub_wallpaper.jpg /boot/grub/

# -------------------- USUARIOS Y CONTRASEÑAS --------------------
# Generar hash de contraseña PBKDF2
sudo grub-mkpasswd-pbkdf2

# Editar configuración de usuarios
sudo nano /etc/grub.d/40_custom

# Ejemplo de líneas para añadir en 40_custom:
# set superusers="grubroot,rootgrub"
# password_pbkdf2 grubroot hash_del_grubroot
# password_pbkdf2 rootgrub hash_del_rootgrub
# password general general

# Editar entradas de Linux para usuario normal
sudo nano /etc/grub.d/10_linux

# -------------------- REINICIAR --------------------
sudo reboot

# -------------------- COMANDOS EN LA CONSOLA DE GRUB (al arrancar) --------------------
# 'e' = editar entrada
# 'c' = consola de comandos de GRUB
# ESC = mostrar menú (si está oculto)
