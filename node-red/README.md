# Node-RED Integration

Folder ini berisi integrasi awal Node-RED untuk Smart Parking System.

## Fitur pada tahap awal

- POST /api/rfid/scan menerima UID kartu dan gate_id, lalu mencatat hasil pemindaian ke SQLite.
- GET /api/health memeriksa apakah flow HTTP Node-RED aktif.
- Skema database tersedia pada sql/schema.sql.
- Flow yang dapat diimpor tersedia pada flows/parking-system.json.

Flow ini belum melakukan validasi kartu terdaftar, membuka palang fisik, atau membuat dan menutup sesi parkir. Fitur tersebut akan ditambahkan setelah protokol perangkat dan aturan transaksi disepakati.

## Persyaratan

- Node.js 18 atau lebih baru
- Node-RED 4.x
- Node-RED SQLite node: node-red-node-sqlite

## Instalasi

Jalankan dari folder node-red:

```bash
npm install
mkdir -p data
sqlite3 data/parking.db < sql/schema.sql
npm start
```

Jika SQLite CLI belum tersedia, jalankan isi sql/schema.sql satu kali melalui aplikasi pengelola SQLite.

Buka editor Node-RED pada http://localhost:1880. Pilih Menu → Import, lalu impor flows/parking-system.json. Pada konfigurasi SQLite, atur lokasi database ke path absolut node-red/data/parking.db di komputer yang menjalankan Node-RED. Setelah itu tekan Deploy.

## API

### Pemeriksaan layanan

```http
GET http://localhost:1880/api/health
```

### Mencatat pemindaian RFID

```http
POST http://localhost:1880/api/rfid/scan
Content-Type: application/json

{
  "gate_id": "entry",
  "uid": "A1B2C3D4"
}
```

gate_id hanya menerima entry atau exit. UID dinormalisasi menjadi huruf kapital.

Contoh pengujian:

```bash
curl -X POST http://localhost:1880/api/rfid/scan \
  -H "Content-Type: application/json" \
  -d '{"gate_id":"entry","uid":"A1B2C3D4"}'
```

## Catatan keamanan dan batasan

- Query SQL menggunakan parameter agar input UID tidak digabung langsung ke string SQL.
- API tahap awal belum memakai autentikasi dan hanya ditujukan untuk jaringan pengembangan lokal. Jangan membuka port 1880 langsung ke internet.
- Pemindaian RFID bukan bukti kendaraan telah melewati palang.
- Status palang fisik hanya boleh dianggap terverifikasi setelah ESP32 mengirimkan konfirmasi status.
- File database pada folder data merupakan data runtime dan tidak perlu di-commit ke Git.
