//========================================================================
//
// Catalog.h
//
// Copyright 1996-2007 Glyph & Cog, LLC
//
//========================================================================

#ifndef CATALOG_H
#define CATALOG_H

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#if MULTITHREADED
#include "GMutex.h"
#endif
#include "CharTypes.h"

class GList;
class PDFDoc;
class XRef;
class Object;
class Page;
class PageAttrs;
struct Ref;
class LinkDest;
class PageTreeNode;
class Form;
class TextString;

//------------------------------------------------------------------------
// Catalog
//------------------------------------------------------------------------

class Catalog {
public:

  // Constructor.
  Catalog(PDFDoc *docA);

  // Destructor.
  ~Catalog();

  // Is catalog valid?
  GBool isOk() { return ok; }

  // Get number of pages.
  int getNumPages() { return numPages; }

  // Get a page.
  Page *getPage(int i);

  // Get the reference for a page object.
  Ref *getPageRef(int i);

  // Remove a page from the catalog.  (It can be reloaded later by
  // calling getPage).
  void doneWithPage(int i);

  // Return base URI, or NULL if none.
  GString *getBaseURI() { return baseURI; }

  // Return the contents of the metadata stream, or NULL if there is
  // no metadata.
  GString *readMetadata();

  // Return the structure tree root object.
  Object *getStructTreeRoot() { return &structTreeRoot; }

  // Find a page, given its object ID.  Returns page number, or 0 if
  // not found.
  int findPage(int num, int gen);

  // Find a named destination.  Returns the link destination, or
  // NULL if <name> is not a destination.
  LinkDest *findDest(GString *name);

  Object *getDests() { return &dests; }

  Object *getNameTree() { return &nameTree; }

  Object *getOutline() { return &outline; }

  Object *getAcroForm() { return &acroForm; }

  Form *getForm() { return form; }

  GBool getNeedsRendering() { return needsRendering; }

  Object *getOCProperties() { return &ocProperties; }

  // Return the DestOutputProfile stream, or NULL if there isn't one.
  Object *getDestOutputProfile(Object *destOutProf);

#ifndef NO_EMBEDDED_CONTENT
  // Get the list of embedded files.
  int getNumEmbeddedFiles();
  Unicode *getEmbeddedFileName(int idx);
  int getEmbeddedFileNameLength(int idx);
  Object *getEmbeddedFileStreamRef(int idx);
  Object *getEmbeddedFileStreamObj(int idx, Object *strObj);
#endif
  // Get the page label for page number [pageNum].  Returns NULL if
  // the PDF file doesn't have page labels.
  TextString *getPageLabel(int pageNum);

private:

  PDFDoc *doc;
  XRef *xref;			// the xref table for this PDF file
  PageTreeNode *pageTree;	// the page tree
  Page **pages;			// array of pages
  Ref *pageRefs;		// object ID for each page
#if MULTITHREADED
  GMutex pageMutex;
#endif
  int numPages;			// number of pages
  Object dests;			// named destination dictionary
  Object nameTree;		// name tree
  GString *baseURI;		// base URI for URI-type links
  Object metadata;		// metadata stream
  Object structTreeRoot;	// structure tree root dictionary
  Object outline;		// outline dictionary
  Object acroForm;		// AcroForm dictionary
  GBool needsRendering;		// NeedsRendering flag
  Form *form;			// parsed form
  Object ocProperties;		// OCProperties dictionary
  GList *embeddedFiles;		// embedded file list [EmbeddedFile]
  GBool ok;			// true if catalog is valid

  Object *findDestInTree(Object *tree, GString *name, Object *obj);
  GBool readPageTree(Object *catDict);
  int countPageTree(Object *pagesObj);
  void loadPage(int pg);
  void loadPage2(int pg, int relPg, PageTreeNode *node);
#ifndef NO_EMBEDDED_CONTENT
  void readEmbeddedFileList(Dict *catDict);
  void readEmbeddedFileTree(Object *node);
  void readFileAttachmentAnnots(Object *pageNodeRef,
				char *touchedObjs);
  void readEmbeddedFile(Object *fileSpec, Object *name1);
#endif
  GBool findPageLabel(Object *node, int pageIndex,
		      Object *pageLabelObj, int *firstPageIndex);
  GString *makeRomanNumeral(int num, GBool uppercase);
  GString *makeLetterLabel(int num, GBool uppercase);
};

#endif
