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

# ============================================
# CHULETA DE COMANDOS DE CIFRADO LUKS
# ============================================

# -------------------- VISUALIZACIÓN DE DISCOS --------------------
# Abrir administrador de discos gráfico
sudo gnome-disks

# Ver particiones del sistema
lsblk

# Ver información detallada de particiones
sudo blkid

# -------------------- CIFRADO DE PARTICIÓN CON LUKS --------------------
# Cifrar partición (ej: /dev/sda8) - MÉTODO GRÁFICO:
# 1. Abrir gnome-disks
# 2. Seleccionar partición
# 3. Click engranaje → Formatear partición
# 4. Tipo: "Cifrada con LUKS (Linux)"
# 5. Establecer contraseña segura

# Cifrar partición - MÉTODO TERMINAL:
sudo cryptsetup luksFormat /dev/sda8

# Abrir/desbloquear partición cifrada (crea mapeador)
sudo cryptsetup luksOpen /dev/sda8 nombre_mapeador

# Ejemplo:
sudo cryptsetup luksOpen /dev/sda8 datos_cif

# -------------------- VER MAPEADORES ACTIVOS --------------------
ls /dev/mapper

# -------------------- FORMATEAR Y MONTAR PARTICION DESBLOQUEADA --------------------
# Formatear con sistema de archivos (después de luksOpen)
sudo mkfs.ext4 /dev/mapper/nombre_mapeador

# Crear punto de montaje
sudo mkdir -p /media/datos_cifrados

# Montar la partición desbloqueada
sudo mount /dev/mapper/nombre_mapeador /media/datos_cifrados

# -------------------- CERRAR/BLOQUEAR PARTICION --------------------
# Bloquear/cerrar partición (debe estar desmontada)
sudo umount /media/datos_cifrados
sudo cryptsetup luksClose /dev/mapper/nombre_mapeador

# Ejemplo con nombre específico:
sudo cryptsetup luksClose /dev/mapper/luks-e4866841-1240-49b4-869e-e9e5f20b8456

# -------------------- DESBLOQUEAR DESDE TERMINAL --------------------
# Abrir partición (solicitará contraseña)
sudo cryptsetup luksOpen /dev/sda8 nombre_mapeador

# Montar después de abrir
sudo mount /dev/mapper/nombre_mapeador /media/datos_cifrados

# -------------------- CREAR ARCHIVOS DE PRUEBA --------------------
# Crear archivo dentro de la partición cifrada
echo "contenido_secreto" | sudo tee /media/datos_cifrados/archivo.txt

# Ver contenido
sudo cat /media/datos_cifrados/archivo.txt

# -------------------- COMANDOS ÚTILES ADICIONALES --------------------
# Ver estado de la partición LUKS
sudo cryptsetup status nombre_mapeador

# Ver información del header LUKS
sudo cryptsetup luksDump /dev/sda8

# Añadir otra contraseña a la partición (key slot)
sudo cryptsetup luksAddKey /dev/sda8

# Eliminar una contraseña (indicar key slot)
sudo cryptsetup luksRemoveKey /dev/sda8

# Ver key slots disponibles
sudo cryptsetup luksDump /dev/sda8 | grep Slot

# -------------------- ACCESO DESDE LIVE CD --------------------
# En un sistema live (Kali/Ubuntu), para acceder a partición cifrada:
# 1. Identificar la partición cifrada
lsblk

# 2. Desbloquearla
sudo cryptsetup luksOpen /dev/sda8 datos_cif

# 3. Montarla
sudo mkdir -p /mnt/cifrado
sudo mount /dev/mapper/datos_cif /mnt/cifrado

# 4. Acceder a los datos
ls -la /mnt/cifrado

# -------------------- RESOLUCIÓN DE PROBLEMAS --------------------
# Error: "device already exists" - cerrar mapeador existente
sudo cryptsetup luksClose /dev/mapper/nombre_mapeador

# Error: "target is busy" - desmontar primero
sudo umount /media/datos_cifrados

# Forzar desmontaje si está ocupado
sudo umount -l /media/datos_cifrados

# -------------------- DIFERENCIA: DATOS CIFRADOS vs NO CIFRADOS --------------------
# Partición NO cifrada desde Live CD:
# - Acceso inmediato con sudo
# - Confidencialidad: COMPROMETIDA
# - Integridad: COMPROMETIDA

# Partición cifrada desde Live CD:
# - Solicita contraseña de cifrado
# - Sin contraseña: ACCESO IMPOSIBLE
# - Segura contra acceso físico al disco
