/* * * * * * * * * * * * * * * * * * * * *
**
** Copyright 2012 Dominik Pretzsch
** 
**    Licensed under the Apache License, Version 2.0 (the "License");
**    you may not use this file except in compliance with the License.
**    You may obtain a copy of the License at
** 
**        http://www.apache.org/licenses/LICENSE-2.0
** 
**    Unless required by applicable law or agreed to in writing, software
**    distributed under the License is distributed on an "AS IS" BASIS,
**    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
**    See the License for the specific language governing permissions and
**    limitations under the License.
**
** * * * * * * * * * * * * * * * * * * */

#ifndef WIN32_NO_STATUS
#include <ntstatus.h>
#define WIN32_NO_STATUS
#endif
#include <unknwn.h>
#include "CMultiOneTimePasswordCredentialProviderFilter.h"
#include "guid.h"

// Boilerplate code to create our provider.
HRESULT CSample_CreateInstance(__in REFIID riid, __deref_out void** ppv)
{
    HRESULT hr;

    CMultiOneTimePasswordCredentialProviderFilter* pProvider = new CMultiOneTimePasswordCredentialProviderFilter();

    if (pProvider)
    {
        hr = pProvider->QueryInterface(riid, ppv);
        pProvider->Release();
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }
    
    return hr;
}


HRESULT CMultiOneTimePasswordCredentialProviderFilter::Filter(CREDENTIAL_PROVIDER_USAGE_SCENARIO cpus,DWORD dwFlags,GUID* rgclsidProviders,BOOL* rgbAllow,DWORD cProviders)
{
	switch (cpus) 
    { 
        case CPUS_LOGON: 
		case CPUS_UNLOCK_WORKSTATION:
			for (DWORD i = 0; i < cProviders; i++) 
            { 
				if ( i < dwFlags )
				{ } 
				//if (IsEqualGUID(rgclsidProviders[i], CLSID_PasswordCredentialProvider))
				// Only allow OTP CPs (Logon and Resync)
				if (IsEqualGUID(rgclsidProviders[i], CLSID_COTP_LOGON) || IsEqualGUID(rgclsidProviders[i], CLSID_COTP_RESYNC)) {
					rgbAllow[i] = TRUE; 
				} else {
					rgbAllow[i] = FALSE; 
				}
            }
            return S_OK; 
			break;         
        case CPUS_CREDUI: 
        case CPUS_CHANGE_PASSWORD: 
            return E_NOTIMPL; 
        default: 
            return E_INVALIDARG; 
    }     
}

CMultiOneTimePasswordCredentialProviderFilter::CMultiOneTimePasswordCredentialProviderFilter():
    _cRef(1)
{
    DllAddRef();
}

CMultiOneTimePasswordCredentialProviderFilter::~CMultiOneTimePasswordCredentialProviderFilter()
{
	DllRelease();
}

HRESULT CMultiOneTimePasswordCredentialProviderFilter::UpdateRemoteCredential( const CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION *pcpsIn , CREDENTIAL_PROVIDER_CREDENTIAL_SERIALIZATION *pcpcsOut)
{
	UNREFERENCED_PARAMETER(pcpsIn);
	UNREFERENCED_PARAMETER(pcpcsOut);
	return E_NOTIMPL;
}