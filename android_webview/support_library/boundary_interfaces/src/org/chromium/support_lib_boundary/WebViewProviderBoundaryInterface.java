// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.support_lib_boundary;

import android.net.Uri;
import android.webkit.WebChromeClient;
import android.webkit.WebViewClient;

import java.lang.reflect.InvocationHandler;

/**
 */
public interface WebViewProviderBoundaryInterface {
    void insertVisualStateCallback(long requestId,
            /* VisualStateCallback */ InvocationHandler callback);
    /* WebMessagePort */ InvocationHandler[] createWebMessageChannel();
    void postMessageToMainFrame(/* WebMessage */ InvocationHandler message, Uri targetOrigin);
    WebViewClient getWebViewClient();
    WebChromeClient getWebChromeClient();
    /* WebViewRenderer */ InvocationHandler getWebViewRenderer();
}
