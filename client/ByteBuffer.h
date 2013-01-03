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
        ByteBuffer &operator<<(const string &value)
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

		// Operateur >> bool, byte, short, int, long
        ByteBuffer &operator>>(bool &value)
        {
            value = read<char>() > 0 ? true : false;
            return *this;
        }

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

        byte operator[](size_t pos) const
        {
            return read<byte>(pos);
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

        void resize(size_t newsize)
        {
            storage.resize(newsize, 0);
            _rpos = 0;
            _wpos = size();
        }

        void reserve(size_t ressize)
        {
            if (ressize > size())
                storage.reserve(ressize);
        }

        void clear()
        {
            storage.clear();
            _rpos = _wpos = 0;
        }

		// IO templates
        template <typename T> void append(T value)
        {
            EndianConvert(value);
            append((byte *)&value, sizeof(value));
        }

        template <typename T> void put(size_t pos, T value)
        {
            EndianConvert(value);
            put(pos, (byte *)&value, sizeof(value));
        }

		template <typename T> T read()
        {
            T r = read<T>(_rpos);
            _rpos += sizeof(T);

            return r;
        }

        template <typename T> T read(size_t pos) const
        {
            if (pos + sizeof(T) > size())
                throw exception("read(pos) : depassement buffer");

            T val = *((T const*)&storage[pos]);
            EndianConvert(val);

            return val;
        }

		template<typename T> void read_skip()
		{
			read_skip(sizeof(T));
		}

        void read_skip(size_t skip)
        {
            if (_rpos + skip > size())
                throw exception("read_skip : depassement buffer");

            _rpos += skip;
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

		// Ecriture : pointeur char
		void append(const char* src, size_t cnt)
        {
            return append((const byte *)src, cnt);
        }

		// Ecriture : Autre ByteBuffer
		void append(const ByteBuffer& buffer)
        {
            if (buffer.wpos())
                append(buffer.contents(), buffer.wpos());
        }

		// Ecriture positionnée ; pointeur d'octets
        void put(size_t pos, const byte *src, size_t cnt)
        {
            if (pos + cnt > size())
                throw exception("put : depassement buffer");

            if (!src)
                throw exception("put : src manquant");

            memcpy(&storage[pos], src, cnt);
        }

		// Lecture position courante _rpos dans *dest
        void read(byte *dest, size_t len)
        {
            if (_rpos  + len > size())
               throw exception("read : depassement buffer");

            memcpy(dest, &storage[_rpos], len);
            _rpos += len;
		}

    private:
        size_t _rpos, _wpos;
        vector<byte> storage;
};

#endif
