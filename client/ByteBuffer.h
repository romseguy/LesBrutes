#ifndef _BYTEBUFFER_H
#define _BYTEBUFFER_H

// Gère un vecteur d'octets avec _rpos et _wpos indiquant le début et la fin
class ByteBuffer
{
	public:
		 static const size_t DEFAULT_SIZE = 0x1000; // 1ko
		 static const size_t MAX_SIZE = 0x10000000; // 10mo

		// constructor
		ByteBuffer(): _rpos(0), _wpos(0) { storage.reserve(DEFAULT_SIZE); }
		ByteBuffer(size_t res): _rpos(0), _wpos(0) { storage.reserve(res); }

		// copy constructor
		ByteBuffer(const ByteBuffer &buf): _rpos(buf._rpos), _wpos(buf._wpos), storage(buf.storage) {}

		
		// Operateur << byte,short,int,long
		ByteBuffer &operator<<(byte value)
		{
			append<byte>(value);
			return *this;
		}

		// Operateur >> byte,short,int,long
		ByteBuffer &operator>>(byte &value)
		{
			value = read<byte>();
			return *this;
		}

		// Chaînes de caractères
		ByteBuffer &operator>>(string &value)
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

		// Accesseurs _rpos _wpos
		size_t rpos() const { return _rpos; }
		size_t wpos() const { return _wpos; }
		size_t rpos(size_t rpos_) { _rpos = rpos_; return _rpos; }
		size_t wpos(size_t wpos_) { _wpos = wpos_; return _wpos; }

		// Accesseurs vecteur
		const byte* contents() const { return &storage[0]; }
		size_t size() const { return storage.size(); }
		bool empty() const { return storage.empty(); }

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
				throw exception("append : cnt manquant");

			if (!src)
				throw exception("append : src manquant");

			if (size() > MAX_SIZE)
				throw exception("append : depassement size");

			// On aggrandit le vecteur si nécessaire
			if (storage.size() < _wpos + cnt)
				storage.resize(_wpos + cnt);

			memcpy(&storage[_wpos], src, cnt);
			_wpos += cnt;
		}

		// Lecture
		template <typename T> T read()
		{
			T r = read<T>(_rpos);
			_rpos += sizeof(T);

			return r;
		}

		// Lecture : position
		template <typename T> T read(size_t pos) const
		{
			if (pos + sizeof(T) > size())
				throw exception("read(pos) : depassement buffer");

			T val = *((T const*)&storage[pos]);
			EndianConvert(val);

			return val;
		}

	private:
		size_t _rpos, _wpos;
		vector<byte> storage;
};

#endif
