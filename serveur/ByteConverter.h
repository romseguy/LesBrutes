#ifndef BYTECONVERTER_H
#define BYTECONVERTER_H

// G�re les conversions bits par bits big/little Endian
// On utilise inline car les fonctions sont simples et souvent appel�es
namespace ByteConverter
{
	// Fonction r�cursive, d'o� le passage par apply
	template<size_t T>
	inline void convert(char *val)
	{
		std::swap(*val, *(val + T - 1));
		convert<T - 2>(val + 1);
	}

	// Pas besoin de traiter les bits centraux
	template<> inline void convert<0>(char *) {}
	template<> inline void convert<1>(char *) {}

	// M�thode d'entr�e pour la conversion d'une valeur de type T
	template<typename T>
	inline void apply(T *val)
	{
		// que l'on cast en char* pour jouer sur les bits avec std::swap
		convert<sizeof(T)>((char *)(val));
	}
}

template<typename T>
inline void EndianConvert(T& val) { ByteConverter::apply<T>(&val); }

#endif
