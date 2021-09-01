/*
 * Copyright (C) 2021 Alibaba Inc. All rights reserved.
 * Author: Kraken Team.
 */

#ifndef KRAKENBRIDGE_ELEMENT_H
#define KRAKENBRIDGE_ELEMENT_H

#include "node.h"
#include "style_declaration.h"
#include "bindings/qjs/host_object.h"
#include <unordered_map>

namespace kraken::binding::qjs {

void bindElement(std::unique_ptr<JSContext> &context);

class ElementInstance;

class Element;

using ElementCreator = ElementInstance *(*)(Element *element, std::string tagName);

struct NativeBoundingClientRect {
  double x;
  double y;
  double width;
  double height;
  double top;
  double right;
  double bottom;
  double left;
};

class ElementAttributes : public HostObject {
public:
  ElementAttributes() = delete;
  explicit ElementAttributes(JSContext *context) : HostObject(context, "ElementAttributes") {}
  ~ElementAttributes();

  JSAtom getAttribute(std::string &name);
  JSValue setAttribute(std::string &name, JSAtom value);
  bool hasAttribute(std::string &name);
  void removeAttribute(std::string &name);
  void copyWith(ElementAttributes *attributes);

private:
  std::unordered_map<std::string, JSAtom> m_attributes;
};

class Element : public Node {
public:
  static JSClassID kElementClassId;
  Element() = delete;
  explicit Element(JSContext *context);

  static JSClassID classId();

  JSValue constructor(QjsContext *ctx, JSValue func_obj, JSValue this_val, int argc, JSValue *argv) override;

  static JSValue getBoundingClientRect(QjsContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
  static JSValue hasAttribute(QjsContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
  static JSValue setAttribute(QjsContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
  static JSValue getAttribute(QjsContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
  static JSValue removeAttribute(QjsContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
  static JSValue toBlob(QjsContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
  static JSValue click(QjsContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
  static JSValue scroll(QjsContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);
  static JSValue scrollBy(QjsContext *ctx, JSValueConst this_val, int argc, JSValueConst *argv);

  static void defineElement(const std::string &tagName, Element *constructor);
  static Element* getConstructor(JSContext *context, const std::string &tagName);

  static std::unordered_map<std::string, Element *> elementConstructorMap;

  OBJECT_INSTANCE(Element);

private:
  ObjectFunction m_getBoundingClientRect{m_context, m_prototypeObject, "getBoundingClientRect", getBoundingClientRect,
                                         0};
  ObjectFunction m_hasAttribute{m_context, m_prototypeObject, "hasAttribute", hasAttribute, 1};
  ObjectFunction m_setAttribute{m_context, m_prototypeObject, "setAttribute", setAttribute, 2};
  ObjectFunction m_getAttribute{m_context, m_prototypeObject, "getAttribute", getAttribute, 2};
  ObjectFunction m_removeAttribute{m_context, m_prototypeObject, "removeAttribute", removeAttribute, 1};
  ObjectFunction m_toBlob{m_context, m_prototypeObject, "toBlob", toBlob, 0};
  ObjectFunction m_click{m_context, m_prototypeObject, "click", click, 0};
  ObjectFunction m_scroll{m_context, m_prototypeObject, "scroll", scroll, 2};
  ObjectFunction m_scrollTo{m_context, m_prototypeObject, "scrollTo", scroll, 2};
  ObjectFunction m_scrollBy{m_context, m_prototypeObject, "scrollBy", scrollBy, 2};
  friend ElementInstance;
};

struct PersistElement {
  ElementInstance *element;
  list_head link;
};

class ElementInstance : public NodeInstance {
public:
  ElementInstance() = delete;
  ~ElementInstance() override {
  }
  JSValue internalGetTextContent() override;
  void internalSetTextContent(JSValue content) override;

  std::string tagName();
  std::string getRegisteredTagName();

  static inline JSClassID classID();

protected:
  explicit ElementInstance(Element *element, std::string tagName, bool shouldAddUICommand);

private:
  DEFINE_HOST_CLASS_PROPERTY(15, nodeName, tagName, offsetLeft, offsetTop, offsetWidth, offsetHeight,
                             clientWidth, clientHeight, clientTop, clientLeft, scrollTop, scrollLeft, scrollHeight,
                             scrollWidth, children);
  void _notifyNodeRemoved(NodeInstance *node) override;
  void _notifyChildRemoved();
  void _notifyNodeInsert(NodeInstance *insertNode) override;
  void _notifyChildInsert();
  void _didModifyAttribute(std::string &name, JSAtom oldId, JSAtom newId);
  void _beforeUpdateId(JSAtom oldId, JSAtom newId);

  PersistElement m_persist_link{this};
  std::string m_tagName;
  friend Element;
  friend NodeInstance;
  friend Node;
  friend DocumentInstance;
  StyleDeclarationInstance *m_style{nullptr};
  ElementAttributes *m_attributes{nullptr};

  std::unordered_map<JSAtom, JSValue> m_properties;

  static JSValue getProperty(QjsContext *ctx, JSValueConst obj, JSAtom atom,
                             JSValueConst receiver);
  static int setProperty(QjsContext *ctx, JSValueConst obj, JSAtom atom,
                         JSValueConst value, JSValueConst receiver, int flags);

  static JSClassExoticMethods exoticMethods;
};

class BoundingClientRect : public HostObject {
public:
  BoundingClientRect() = delete;
  explicit BoundingClientRect(JSContext *context, NativeBoundingClientRect *nativeBoundingClientRect) : HostObject(
    context, "BoundingClientRect"), m_nativeBoundingClientRect(nativeBoundingClientRect) {

  };

  DEFINE_HOST_OBJECT_PROPERTY(8, x, y, width, height, top, right, bottom, left);

private:
  NativeBoundingClientRect *m_nativeBoundingClientRect{nullptr};
};

} // namespace kraken::binding::qjs

#endif // KRAKENBRIDGE_ELEMENT_H