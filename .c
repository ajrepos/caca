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

# ============================================
# CHULETA BITLOCKER - Comandos y gestión
# ============================================

# -------------------- ADMINISTRACIÓN DE DISCOS --------------------
# Abrir administración de discos
diskmgmt.msc

# Inicializar disco (diskpart)
diskpart
list disk
select disk X
online disk
attributes disk clear readonly
convert mbr
create partition primary
format fs=ntfs quick
assign letter=Z
exit

# -------------------- GESTIÓN BITLOCKER --------------------
# Activar BitLocker en unidad
manage-bde -on X: -password -recoverypassword

# Desbloquear unidad con contraseña
manage-bde -unlock X: -password

# Desbloquear con clave de recuperación
manage-bde -unlock X: -recoverypassword CLAVE48DIGITOS

# Bloquear unidad (cifrar de nuevo)
manage-bde -lock X:

# Ver estado de BitLocker
manage-bde -status

# Ver protectores configurados
manage-bde -protectors -get X:

# Eliminar contraseña
manage-bde -protectors -delete X: -type password

# Añadir clave de recuperación
manage-bde -protectors -add X: -recoverypassword

# -------------------- POLÍTICA SIN TPM --------------------
gpedit.msc
# Ruta: Configuración equipo → Plantillas administrativas → Componentes Windows → Cifrado unidad BitLocker → Unidades del sistema operativo
# Opción: "Requerir autenticación adicional al iniciar" → Habilitar → "Permitir BitLocker sin TPM"

# -------------------- DESBLOQUEO AUTOMÁTICO --------------------
# Habilitar desbloqueo automático (unidad de datos)
manage-bde -autounlock -enable X:

# Deshabilitar desbloqueo automático
manage-bde -autounlock -disable X:

# Script batch para desbloqueo automático (tarea programada)
echo manage-bde -unlock X: -password MIPASSWORD > C:\scripts\unlock.bat

# -------------------- DISKPART (preparar pendrive) --------------------
diskpart
list disk
select disk X
clean
create partition primary
format fs=ntfs quick
active
exit

# -------------------- CIFRADO CON DISLOCKER (Linux) --------------------
# Instalar dislocker
sudo apt update && sudo apt install dislocker -y

# Crear puntos de montaje
sudo mkdir -p /media/bitlocker /media/desbloqueado

# Desbloquear y montar
sudo dislocker -V /dev/sdX1 -u -- /media/bitlocker
sudo mount -o loop /media/bitlocker/dislocker-file /media/desbloqueado

# Con contraseña directa
sudo dislocker -V /dev/sdX1 -p CONTRASEÑA -- /media/bitlocker

# -------------------- COMANDOS ÚTILES --------------------
# Suspender BitLocker (actualizaciones)
manage-bde -protectors -disable X:
manage-bde -protectors -enable X:

# Resumen de comandos manage-bde
manage-bde -?                      # Ayuda
manage-bde -status                 # Estado de todas las unidades
manage-bde -unlock X: -password    # Desbloquear con contraseña
manage-bde -lock X:                # Bloquear unidad
manage-bde -autounlock -enable X:  # Desbloqueo automático

# ============================================
# CHULETA GPG - Cifrado y firmas en Linux
# ============================================

# -------------------- CIFRADO SIMÉTRICO --------------------
# Cifrar archivo (pide contraseña)
gpg -c archivo.txt

# Cifrar archivo con contraseña en línea (inseguro, solo pruebas)
echo "secreto" | gpg -c --batch --passphrase "miclave" > archivo.gpg

# Descifrar archivo (pide contraseña)
gpg -d archivo.txt.gpg > archivo_descifrado.txt

# Descifrar ignorando error MDC (archivos antiguos)
gpg --ignore-mdc-error -d archivo.gpg > archivo.txt

# Ver tamaños de archivos
ls -lh archivo.txt archivo.txt.gpg


# -------------------- CIFRADO ASIMÉTRICO --------------------
# Generar par de claves (interactivo)
gpg --full-generate-key

# Generar claves sin passphrase (automatizado)
gpg --batch --passphrase '' --quick-generate-key "Nombre <email@dominio>" rsa2048 sign,encr

# Listar claves públicas
gpg --list-keys

# Listar claves privadas
gpg --list-secret-keys

# Exportar clave pública (ASCII armor)
gpg --export --armor "email@dominio" > clave_publica.asc

# Importar clave pública
gpg --import clave_publica.asc

# Cifrar archivo para un destinatario
gpg --encrypt --recipient "email@destinatario" archivo.txt

# Descifrar archivo
gpg --decrypt archivo.txt.gpg

# Descifrar guardando en archivo
gpg --decrypt archivo.txt.gpg > archivo_descifrado.txt


# -------------------- FIRMAS DIGITALES --------------------
# Firma separada (archivo.sig)
gpg --detach-sign documento.txt

# Verificar firma separada
gpg --verify documento.txt.sig documento.txt

# Firma integrada (clearsign - texto legible)
gpg --clearsign documento.txt

# Firma en binario
gpg --sign documento.txt

# Verificar firma integrada
gpg --verify documento.txt.asc


# -------------------- GESTIÓN DE CLAVES --------------------
# Eliminar clave pública
gpg --delete-key "email@dominio"

# Eliminar clave privada
gpg --delete-secret-key "email@dominio"

# Exportar clave privada (CUIDADO)
gpg --export-secret-key --armor "email@dominio" > clave_privada.asc

# Importar clave privada
gpg --import clave_privada.asc

# Firmar una clave (confianza)
gpg --sign-key "email@dominio"

# Editar clave (modo interactivo)
gpg --edit-key "email@dominio"
# Comandos dentro del modo: trust, sign, adduid, deluid, expire, quit


# -------------------- COMANDOS ÚTILES ADICIONALES --------------------
# Mostrar huella digital de una clave
gpg --fingerprint "email@dominio"

# Exportar clave pública en formato binario (sin --armor)
gpg --export "email@dominio" > clave_publica.gpg

# Cifrado simétrico con algoritmo específico
gpg -c --cipher-algo AES256 archivo.txt

# Ver información detallada de un archivo GPG
gpg --list-packets archivo.gpg

# ============================================
# CHULETA VLSM ACTUALIZADA
# Linux (git) | Windows (curl)
# Herramientas: netcalc, SubNetter
# ============================================


# -------------------- SUBNETTER --------------------

# LINUX (git)
git clone https://github.com/0xShakhawat/subnetter.git
cd subnetter
pip3 install tabulate
python3 subnetter.py 192.168.1.0/24 -s 10,10,10 -t

# WINDOWS (curl)
curl -L -o subnetter.zip "https://github.com/0xShakhawat/subnetter/archive/refs/heads/main.zip"
tar -xf subnetter.zip
cd subnetter-main
pip install tabulate
python subnetter.py 192.168.1.0/24 -s 10,10,10 -t


# -------------------- NETCALC --------------------

# LINUX (git)
git clone https://gist.github.com/Letder40/netcalc
cd netcalc
gcc -o netcalc netcalc.c
./netcalc -v 192.168.1.0 255.255.255.0

# WINDOWS (curl) - Necesita MinGW (gcc)
curl -L -o netcalc.c "https://gist.githubusercontent.com/Letder40/raw/netcalc.c"
gcc -o netcalc.exe netcalc.c
netcalc.exe -v 192.168.1.0 255.255.255.0


# -------------------- EJEMPLO VLSM (192.168.1.0/24 → 3 subredes de 10 hosts) --------------------

# SubNetter
python3 subnetter.py 192.168.1.0/24 -s 10,10,10 -t

# netcalc (NO hace VLSM automático, solo muestra info de la red)
netcalc -v 192.168.1.0 255.255.255.0


# -------------------- OBSERVACIONES --------------------
# - SubNetter: Sí soporta VLSM con -s
# - netcalc: No soporta VLSM, solo muestra información de subred simple
# - ip-calculator: No incluido porque no tiene Windows con curl
# - ipcalc: Solo Linux, no Windows


# ============================================
# CHULETA VLAN - Cisco Packet Tracer
# ============================================

# -------------------- COMANDOS BÁSICOS SWITCH --------------------
enable                          # Modo privilegiado
configure terminal              # Modo configuración
exit                            # Salir del modo actual
write memory                    # Guardar configuración
show vlan brief                 # Ver VLANs y puertos asignados
show interfaces trunk           # Ver puertos trunk
show running-config             # Ver configuración actual
show ip interface brief         # Ver interfaces IP (router)

# -------------------- CREAR VLANs --------------------
vlan 10
name Cuerpo_docente
exit

vlan 20
name Estudiante
exit

vlan 30
name Invitado
exit

# -------------------- CONFIGURAR PUERTOS ACCESO --------------------
interface fastEthernet 0/11
switchport mode access
switchport access vlan 10
exit

# Asignar VLAN en caliente (sin salir del modo interfaz)
switchport access vlan 20

# -------------------- CONFIGURAR PUERTOS TRUNK --------------------
interface fastEthernet 0/1
switchport mode trunk
exit

# Configurar trunk con VLAN nativa (opcional)
interface fastEthernet 0/1
switchport trunk native vlan 1
switchport trunk allowed vlan 10,20,30

# -------------------- CONFIGURAR ROUTER (Router-on-a-Stick) --------------------
interface fastEthernet 0/1
no shutdown
exit

# Subinterfaz VLAN 10
interface fastEthernet 0/1.10
encapsulation dot1q 10
ip address 172.17.10.1 255.255.255.0
exit

# Subinterfaz VLAN 20
interface fastEthernet 0/1.20
encapsulation dot1q 20
ip address 172.17.20.1 255.255.255.0
exit

# Subinterfaz VLAN 30
interface fastEthernet 0/1.30
encapsulation dot1q 30
ip address 172.17.30.1 255.255.255.0
exit

# -------------------- VERIFICACIONES --------------------
show vlan brief                 # Ver VLANs y puertos
show interfaces trunk           # Ver puertos en modo trunk
show ip route                   # Ver tabla de enrutamiento (router)
show interfaces status          # Ver estado de todos los puertos
show mac address-table          # Ver tabla MAC del switch

# -------------------- COMANDOS ÚTILES ADICIONALES --------------------
# Eliminar VLAN
no vlan 10

# Eliminar configuración de interfaz
default interface fastEthernet 0/11

# Apagar/encender interfaz
shutdown
no shutdown

# Ver detalles de una interfaz específica
show interface fastEthernet 0/1

# Ver configuración trunk detallada
show interface fastEthernet 0/1 switchport

# -------------------- RESOLUCIÓN DE PROBLEMAS --------------------
# Ping desde PC (modo simulación o CLI)
ping 172.17.10.24

# Verificar IP configurada en PC (CLI de PC)
ipconfig

# Comprobar conectividad con gateway
ping 172.17.10.1

# Si no hay comunicación entre VLANs:
# 1. Verificar que subinterfaces están UP
show ip interface brief
# 2. Verificar encapsulación dot1q
show running-config | section interface
# 3. Verificar que switch tiene puerto trunk al router
show interfaces trunk