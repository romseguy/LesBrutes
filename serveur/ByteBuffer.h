#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

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
		ByteBuffer &operator<<(byte value)
		{
			append<byte>(value);
			return *this;
		}

		// unsigned
		ByteBuffer &operator<<(unsigned short value)
		{
			append<unsigned short>(value);
			return *this;
		}

		ByteBuffer &operator<<(unsigned int value)
		{
			append<unsigned int>(value);
			return *this;
		}

		ByteBuffer &operator<<(unsigned long value)
		{
			append<unsigned long>(value);
			return *this;
		}

		// signed
		ByteBuffer &operator<<(short value)
		{
			append<short>(value);
			return *this;
		}

		ByteBuffer &operator<<(int value)
		{
			append<long>(value);
			return *this;
		}

		ByteBuffer &operator<<(long value)
		{
			append<long>(value);
			return *this;
		}

		// Chaînes de caractères
		ByteBuffer &operator<<(const std::string &value)
		{
			if (size_t len = value.length())
				append((byte const*)value.c_str(), len);

			append((byte)0);
			return *this;
		}

		ByteBuffer &operator<<(const char *str)
		{
			if (size_t len = (str ? strlen(str) : 0))
				append((byte const*)str, len);

			append((byte)0);
			return *this;
		}

		// Operateur >> byte,short,int,long
		ByteBuffer &operator>>(byte &value)
		{
			value = read<byte>();
			return *this;
		}

		// unsigned
		ByteBuffer &operator>>(unsigned short &value)
		{
			value = read<unsigned short>();
			return *this;
		}

		ByteBuffer &operator>>(unsigned int &value)
		{
			value = read<unsigned int>();
			return *this;
		}

		ByteBuffer &operator>>(unsigned long &value)
		{
			value = read<unsigned long>();
			return *this;
		}

		// signed
		ByteBuffer &operator>>(short &value)
		{
			value = read<short>();
			return *this;
		}

		ByteBuffer &operator>>(int &value)
		{
			value = read<int>();
			return *this;
		}

		ByteBuffer &operator>>(long &value)
		{
			value = read<long>();
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

		byte operator[](size_t pos) const
		{
			return read<byte>(pos);
		}

		// Accesseurs m_rpos m_wpos
		size_t rpos() const { return m_rpos; }
		size_t wpos() const { return m_wpos; }
		size_t rpos(size_t rpos_) { m_rpos = rpos_; return m_rpos; }
		size_t wpos(size_t wpos_) { m_wpos = wpos_; return m_wpos; }

		// Accesseurs vecteur
		const byte* contents() const { return &storage[0]; }
		const byte* contents(size_t pos) const { return &storage[pos]; }
		size_t size() const { return storage.size(); }
		bool empty() const { return storage.empty(); }

		void resize(size_t newsize)
		{
			storage.resize(newsize, 0);
			m_rpos = 0;
			m_wpos = size();
		}

		// Ecriture
		template <typename T> void append(T value)
		{
			EndianConvert(value);
			append((byte *)&value, sizeof(value));
		}

		// Ecriture : pointeur d'octets
		void append(const byte *src, size_t cnt)
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

		// Ecriture : pointeur char
		void append(const char* src, size_t cnt)
		{
			return append((const byte *)src, cnt);
		}

		// Lecture
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
		std::vector<byte> storage;
};

#endif
