#include <algorithm>

#include "FileStringFindReplace.h"
#include "ChunkIterator.h"
#include "MemAudioFile.h"
#include <vector>

FileStringFindReplace::FileStringFindReplace(const std::vector<char>& sOrig, const std::vector<char> & sNew, const std::string & outFileName)
	: m_searchString(sOrig), m_replaceString(sNew), m_outFile(std::make_shared<MemAudioFile>(outFileName))
{
}

FileStringFindReplace::FileStringFindReplace(const std::vector<char>& sOrig, const std::vector<char> & sNew, const std::shared_ptr<AbsAudioFile>& outFile)
	: m_searchString(sOrig), m_replaceString(sNew), m_outFile(outFile)
{
}

FileStringFindReplace * FileStringFindReplace::clone(void)
{
	return new FileStringFindReplace(*this);
}

void FileStringFindReplace::visit(const AudioFile & f)
{
	// A COMPLETER:
	// Iterer sur chaque Chunk du fichier d'entree
	for (auto it = f.begin(); it != f.end(); it++) {

		//    - Construire un nouvel iterateur pointant a la fin du fichier de sortie
		auto fini = m_outFile->end();

		//    - Initialiser la memoire de l'iterateur avec le contenu du Chunk d'entree
		*fini = *it;

		//    - Remplacer les caracteres dans le Chunk de sortie avec findReplace
		char* char_b = fini->get();
		char* char_e = fini->get();
		char_e += fini->size() - 1;

		findReplace(char_b, char_e);

		//    - Ajouter le nouveau Chunk au fichier de sortie
		m_outFile->addChunk(fini);

	}
}

void FileStringFindReplace::visit(const MemAudioFile & f)
{
	size_t chunkSize = f.getChunkSize();
	size_t nChunks = f.getNumberChunks();
	const char* buf = f.get() + f.getHeaderSize();

	// A COMPLETER:
	// Iterer sur chaque Chunk du fichier d'entree dans buf
	for (int i = 0; i < nChunks; i++) {

		//    - Construire un nouvel iterateur pointant a la fin du fichier de sortie
		auto fini = m_outFile->end();

		//    - Initialiser la memoire de l'iterateur avec le contenu du Chunk d'entree
		Chunk tmpChunk(chunkSize, buf);

		*fini = tmpChunk;

		//    - Remplacer les caracteres dans le Chunk de sortie avec findReplace
		char* char_b = fini->get();
		char* char_e = fini->get();
		char_e += fini->size() - 1;

		findReplace(char_b, char_e);

		//    - Ajouter le nouveau Chunk au fichier de sortie
		m_outFile->addChunk(fini);

		//    - Avancer dans buf
		buf += chunkSize;

	}
}

void FileStringFindReplace::findReplace(char * p_beg, char * p_end)
{
	auto it = std::search(p_beg, p_end, m_searchString.begin(), m_searchString.end());
	if (it != p_end)
	{
		for (size_t irepl = 0; irepl < m_replaceString.size(); ++irepl)
			*(it + irepl) = m_replaceString[irepl];
	}
}
