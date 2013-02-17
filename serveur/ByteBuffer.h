#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include "ByteConverter.h"

// Gère un vecteur d'octets avec m_rpos et m_wpos indiquant le début et la fin
class ByteBuffer
{
	public:
		 static const size_t DEFAULT_SIZE = 0x1000; // 1ko
		 static const size_t MAX_SIZE = 0x10000000; // 10mo

		// constructor
		ByteBuffer(): m_rpos(0), m_wpos(0) { storage.reserve(DEFAULT_SIZE); }
		ByteBuffer(size_t res): m_rpos(0), m_wpos(0) { storage.reserve(res); }

		// copy constructor
		ByteBuffer(const ByteBuffer &buf): m_rpos(buf.m_rpos), m_wpos(buf.m_wpos), storage(buf.storage) {}

		// Operateur << byte,short,int,long
		ByteBuffer &operator<<(uint8_t value)
		{
			append<uint8_t>(value);
			return *this;
		}

		// unsigned
		ByteBuffer &operator<<(uint16_t value)
		{
			append<uint16_t>(value);
			return *this;
		}

		ByteBuffer &operator<<(uint32_t value)
		{
			append<uint32_t>(value);
			return *this;
		}

		ByteBuffer &operator<<(uint64_t value)
		{
			append<uint64_t>(value);
			return *this;
		}

		// signed
		ByteBuffer &operator<<(int16_t value)
		{
			append<int16_t>(value);
			return *this;
		}

		ByteBuffer &operator<<(int32_t value)
		{
			append<int32_t>(value);
			return *this;
		}

		ByteBuffer &operator<<(int64_t value)
		{
			append<int64_t>(value);
			return *this;
		}

		// Chaînes de caractères
		ByteBuffer &operator<<(const std::string &value)
		{
			if (size_t len = value.length())
				append((uint8_t const*) value.c_str(), len);

			append((uint8_t)0);
			return *this;
		}

		ByteBuffer &operator<<(const char *str)
		{
			if (size_t len = (str ? strlen(str) : 0))
				append((uint8_t const*) str, len);

			append((uint8_t)0);
			return *this;
		}

		// Images, fichiers...
		ByteBuffer &operator<<(std::ifstream &fs)
		{
			// taille
			fs.seekg (0, std::ios::end);
			size_t len = fs.tellg();
			fs.seekg (0, std::ios::beg);

			// lecture vers un tableau de caractères pour notre fonction append
			char* buffer = new char[len];
			fs.read(buffer, len);
			fs.close();

			append((uint8_t const*) buffer, len);

			delete[] buffer;

			return *this;
		}

		// Operateur >> byte,short,int,long
		ByteBuffer &operator>>(uint8_t &value)
		{
			value = read<uint8_t>();
			return *this;
		}

		// unsigned
		ByteBuffer &operator>>(uint16_t &value)
		{
			value = read<uint16_t>();
			return *this;
		}

		ByteBuffer &operator>>(uint32_t &value)
		{
			value = read<uint32_t>();
			return *this;
		}

		ByteBuffer &operator>>(uint64_t &value)
		{
			value = read<uint64_t>();
			return *this;
		}

		// signed
		ByteBuffer &operator>>(int16_t &value)
		{
			value = read<int16_t>();
			return *this;
		}

		ByteBuffer &operator>>(int32_t &value)
		{
			value = read<int32_t>();
			return *this;
		}

		ByteBuffer &operator>>(int64_t &value)
		{
			value = read<int64_t>();
			return *this;
		}

		// Chaînes de caractères
		ByteBuffer &operator>>(std::string &value)
		{
			value.clear();

			while (rpos() < size())
			{
				char c = read<char>();

				// fin de chaine
				if (c == 0)
					break;

				value += c;
			}

			return *this;
		}

		uint8_t operator[](size_t pos) const
		{
			return read<uint8_t>(pos);
		}

		// Accesseurs m_rpos m_wpos
		size_t rpos() const { return m_rpos; }
		size_t wpos() const { return m_wpos; }
		size_t rpos(size_t rpos) { m_rpos = rpos; return m_rpos; }
		size_t wpos(size_t wpos) { m_wpos = wpos; return m_wpos; }

		// Accesseurs vecteur
		const uint8_t* contents() const { return &storage[0]; }
		const uint8_t* contents(size_t pos) const { return &storage[pos]; }
		size_t size() const { return storage.size(); }
		bool empty() const { return storage.empty(); }

		void resize(size_t newsize) { storage.resize(newsize, 0); m_rpos = 0; m_wpos = size(); }
		void clear() { storage.clear(); m_rpos = m_wpos = 0; }

		// Ecriture générique
		template <typename T> void append(T value)
		{
			EndianConvert(value);
			append((uint8_t *)&value, sizeof(value));
		}

		// Ecriture : pointeur char
		void append(const char* src, size_t cnt)
		{
			return append((const uint8_t *)src, cnt);
		}

		// Ecriture : pointeur d'octets
		void append(const uint8_t *src, size_t cnt)
		{
			if (!cnt)
				throw std::exception("append : cnt manquant");

			if (!src)
				throw std::exception("append : src manquant");

			if (size() > MAX_SIZE)
				throw std::exception("append : depassement size");

			// On aggrandit le vecteur si nécessaire
			if (storage.size() < m_wpos + cnt)
				storage.resize(m_wpos + cnt);

			memcpy(&storage[m_wpos], src, cnt);
			m_wpos += cnt;
		}

		// Lecture générique
		template <typename T> T read()
		{
			T r = read<T>(m_rpos);
			m_rpos += sizeof(T);

			return r;
		}

		// Lecture : position
		template <typename T> T read(size_t pos) const
		{
			if (pos + sizeof(T) > size())
				throw std::exception("read(pos) : depassement buffer");

			T val = *((T const*) &storage[pos]);
			EndianConvert(val);

			return val;
		}

	private:
		size_t m_rpos, m_wpos;
		std::vector<uint8_t> storage;
};

#endif
