/*
 * Copyright (C) 2020 Alibaba Inc. All rights reserved.
 * Author: Kraken Team.
 */

#include "image_element.h"

namespace kraken::binding::jsc {

void bindImageElement(std::unique_ptr<JSContext> &context) {
  auto ImageElement = JSImageElement::instance(context.get());
  JSC_GLOBAL_SET_PROPERTY(context, "Image", ImageElement->classObject);
}

std::unordered_map<JSContext *, JSImageElement *> &JSImageElement::getInstanceMap() {
  static std::unordered_map<JSContext *, JSImageElement *> instanceMap;
  return instanceMap;
}

JSImageElement *JSImageElement::instance(JSContext *context) {
  auto instanceMap = getInstanceMap();
  if (instanceMap.count(context) == 0) {
    instanceMap[context] = new JSImageElement(context);
  }
  return instanceMap[context];
}
JSImageElement::~JSImageElement() {
  auto instanceMap = getInstanceMap();
  instanceMap.erase(context);
}

JSImageElement::JSImageElement(JSContext *context) : JSElement(context) {}
JSObjectRef JSImageElement::instanceConstructor(JSContextRef ctx, JSObjectRef constructor, size_t argumentCount,
                                                const JSValueRef *arguments, JSValueRef *exception) {
  auto instance = new ImageElementInstance(this);
  return instance->object;
}

JSImageElement::ImageElementInstance::ImageElementInstance(JSImageElement *jsAnchorElement)
  : ElementInstance(jsAnchorElement, "img", false), nativeImageElement(new NativeImageElement(nativeElement)) {
  std::string tagName = "img";
  auto args = buildUICommandArgs(tagName);

  foundation::UICommandTaskMessageQueue::instance(context->getContextId())
    ->registerCommand(eventTargetId, UICommand::createElement, args, 1, nativeImageElement);
}

std::vector<JSStringRef> &JSImageElement::ImageElementInstance::getImageElementPropertyNames() {
  static std::vector<JSStringRef> propertyNames{
    JSStringCreateWithUTF8CString("width"),
    JSStringCreateWithUTF8CString("height"),
    JSStringCreateWithUTF8CString("naturalWidth"),
    JSStringCreateWithUTF8CString("naturalHeight"),
    JSStringCreateWithUTF8CString("src"),
    JSStringCreateWithUTF8CString("loading"),
  };
  return propertyNames;
}

const std::unordered_map<std::string, JSImageElement::ImageElementInstance::ImageProperty> &
JSImageElement::ImageElementInstance::getImageElementPropertyMap() {
  static std::unordered_map<std::string, ImageProperty> propertyMap{{"width", ImageProperty::kWidth},
                                                                    {"height", ImageProperty::kHeight},
                                                                    {"naturalWidth", ImageProperty::kNaturalWidth},
                                                                    {"naturalHeight", ImageProperty::kNaturalHeight},
                                                                    {"src", ImageProperty::kSrc},
                                                                    {"loading", ImageProperty::kLoading}};
  return propertyMap;
}

JSValueRef JSImageElement::ImageElementInstance::getProperty(std::string &name, JSValueRef *exception) {
  auto propertyMap = getImageElementPropertyMap();
  if (propertyMap.count(name) > 0) {
    auto property = propertyMap[name];
    switch (property) {
    case ImageProperty::kWidth: {
      getDartMethod()->flushUICommand();
      return JSValueMakeNumber(_hostClass->ctx, nativeImageElement->getImageWidth(nativeImageElement));
    }
    case ImageProperty::kHeight: {
      getDartMethod()->flushUICommand();
      return JSValueMakeNumber(_hostClass->ctx, nativeImageElement->getImageHeight(nativeImageElement));
    }
    case ImageProperty::kNaturalWidth: {
      getDartMethod()->flushUICommand();
      return JSValueMakeNumber(_hostClass->ctx, nativeImageElement->getImageNaturalWidth(nativeImageElement));
    }
    case ImageProperty::kNaturalHeight: {
      getDartMethod()->flushUICommand();
      return JSValueMakeNumber(_hostClass->ctx, nativeImageElement->getImageNaturalHeight(nativeImageElement));
    }
    case ImageProperty::kSrc: {
      if (_src == nullptr) return nullptr;
      return JSValueMakeString(_hostClass->ctx, _src);
    }
    case ImageProperty::kLoading: {
      if (_loading == nullptr) return nullptr;
      return JSValueMakeString(_hostClass->ctx, _loading);
    }
    }
  }

  return ElementInstance::getProperty(name, exception);
}

void JSImageElement::ImageElementInstance::setProperty(std::string &name, JSValueRef value, JSValueRef *exception) {
  auto propertyMap = getImageElementPropertyMap();

  if (propertyMap.count(name) > 0) {
    auto property = propertyMap[name];
    switch (property) {
    case ImageProperty::kWidth:
    case ImageProperty::kHeight: {
      JSStringRef stringRef = JSValueToStringCopy(_hostClass->ctx, value, exception);
      std::string string = JSStringToStdString(stringRef);
      auto args = buildUICommandArgs(name, string);
      foundation::UICommandTaskMessageQueue::instance(_hostClass->contextId)
        ->registerCommand(eventTargetId, UICommand::setProperty, args, 2, nullptr);
    }
    case ImageProperty::kSrc: {
      _src = JSValueToStringCopy(_hostClass->ctx, value, exception);
      JSStringRetain(_src);

      auto args = buildUICommandArgs(name, JSStringRetain(_src));
      foundation::UICommandTaskMessageQueue::instance(_hostClass->contextId)
        ->registerCommand(eventTargetId, UICommand::setProperty, args, 2, nullptr);
      break;
    }
    case ImageProperty::kLoading: {
      _loading = JSValueToStringCopy(_hostClass->ctx, value, exception);
      JSStringRetain(_loading);

      auto args = buildUICommandArgs(name, JSStringRetain(_loading));
      foundation::UICommandTaskMessageQueue::instance(_hostClass->contextId)
        ->registerCommand(eventTargetId, UICommand::setProperty, args, 2, nullptr);
      break;
    }
    default:
      break;
    }
  } else {
    ElementInstance::setProperty(name, value, exception);
  }
}

void JSImageElement::ImageElementInstance::getPropertyNames(JSPropertyNameAccumulatorRef accumulator) {
  ElementInstance::getPropertyNames(accumulator);

  for (auto &property : getImageElementPropertyNames()) {
    JSPropertyNameAccumulatorAddName(accumulator, property);
  }
}

JSImageElement::ImageElementInstance::~ImageElementInstance() {
  delete nativeImageElement;

  if (_src != nullptr) JSStringRelease(_src);
  if (_loading != nullptr) JSStringRelease(_loading);
}

} // namespace kraken::binding::jsc