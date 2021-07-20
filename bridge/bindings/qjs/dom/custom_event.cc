/*
 * Copyright (C) 2021 Alibaba Inc. All rights reserved.
 * Author: Kraken Team.
 */

#include "custom_event.h"
#include "bindings/qjs/qjs_patch.h"
#include "kraken_bridge.h"

#include <utility>

namespace kraken::binding::qjs {

void bindCustomEvent(std::unique_ptr<JSContext> &context) {
  auto *constructor = CustomEvent::instance(context.get());
  context->defineGlobalProperty("CustomEvent", constructor->classObject);
}

OBJECT_INSTANCE_IMPL(CustomEvent);

JSValue CustomEvent::initCustomEvent(QjsContext *ctx, JSValue this_val, int argc, JSValue *argv) {
  if (argc < 1) {
    return JS_ThrowTypeError(
      ctx, "Failed to execute 'initCustomEvent' on 'CustomEvent': 1 argument required, but only 0 present");
  }

  auto *eventInstance = static_cast<CustomEventInstance *>(JS_GetOpaque(this_val, Event::kEventClassID));
  if (eventInstance == nullptr) {
    return JS_ThrowTypeError(ctx, "Failed to addEventListener: this is not an EventTarget object.");
  }

  JSValue &typeValue = argv[0];
  eventInstance->nativeEvent->type = jsValueToNativeString(ctx, typeValue);

  if (argc <= 2) {
    bool canBubble = JS_ToBool(ctx, argv[1]);
    eventInstance->nativeEvent->bubbles = canBubble ? 1 : 0;
  }

  if (argc <= 3) {
    bool cancelable = JS_ToBool(ctx, argv[2]);
    eventInstance->nativeEvent->cancelable = cancelable ? 1 : 0;
  }

  if (argc <= 4) {
    eventInstance->setDetail(argv[3]);
  }
  return JS_NULL;
}

JSValue CustomEvent::constructor(QjsContext *ctx, JSValue func_obj, JSValue this_val, int argc, JSValue *argv) {
  if (argc < 1) {
    return JS_ThrowTypeError(ctx, "Failed to construct 'CustomEvent': 1 argument required, but only 0 present.");
  }

  JSValue &typeArgsValue = argv[0];
  JSValue customEventInit = JS_NULL;

  if (argc == 2) {
    customEventInit = argv[1];
  }

  std::string customEventType = JS_ToCString(ctx, typeArgsValue);
  auto *customEvent = new CustomEventInstance(CustomEvent::instance(context()), customEventType, customEventInit);
  JS_FreeCString(ctx, customEventType.c_str());

  return customEvent->instanceObject;
}

PROP_GETTER(CustomEvent, detail)(QjsContext *ctx, JSValue this_val, int argc, JSValue *argv) {
  auto *customEventInstance = static_cast<CustomEventInstance *>(JS_GetOpaque(this_val, Event::kEventClassID));
  return customEventInstance->getDetail();
}
PROP_SETTER(CustomEvent, detail)(QjsContext *ctx, JSValue this_val, int argc, JSValue *argv) {
  if (argc == 0) return JS_NULL;
  auto *customEventInstance = static_cast<CustomEventInstance *>(JS_GetOpaque(this_val, Event::kEventClassID));
  customEventInstance->setDetail(argv[0]);
  return JS_NULL;
}

CustomEventInstance::CustomEventInstance(CustomEvent *jsCustomEvent, std::string customEventType, JSValue eventInit)
  : EventInstance(jsCustomEvent, std::move(customEventType), eventInit) {
  if (!JS_IsNull(eventInit)) {
    JSAtom detailKey = JS_NewAtom(m_ctx, "detail");
    if (JS_HasProperty(m_ctx, eventInit, detailKey)) {
      m_detail.setValue(JS_GetProperty(m_ctx, eventInit, detailKey));
    }
  }
}

CustomEventInstance::CustomEventInstance(CustomEvent *jsCustomEvent, NativeCustomEvent *nativeCustomEvent)
  : nativeCustomEvent(nativeCustomEvent), EventInstance(jsCustomEvent, nativeCustomEvent->nativeEvent) {
  JSValue newDetail = JS_NewUnicodeString(jsCustomEvent->context()->runtime(), jsCustomEvent->context()->ctx(),
                                          nativeCustomEvent->detail->string, nativeCustomEvent->detail->length);
  nativeCustomEvent->detail->free();
  m_detail.setValue(newDetail);
}
} // namespace kraken::binding::qjs
