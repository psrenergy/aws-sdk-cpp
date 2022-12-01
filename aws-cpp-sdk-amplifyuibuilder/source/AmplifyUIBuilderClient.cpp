﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/core/utils/DNS.h>
#include <aws/core/utils/logging/LogMacros.h>
#include <aws/core/utils/logging/ErrorMacros.h>

#include <aws/amplifyuibuilder/AmplifyUIBuilderClient.h>
#include <aws/amplifyuibuilder/AmplifyUIBuilderErrorMarshaller.h>
#include <aws/amplifyuibuilder/AmplifyUIBuilderEndpointProvider.h>
#include <aws/amplifyuibuilder/model/CreateComponentRequest.h>
#include <aws/amplifyuibuilder/model/CreateFormRequest.h>
#include <aws/amplifyuibuilder/model/CreateThemeRequest.h>
#include <aws/amplifyuibuilder/model/DeleteComponentRequest.h>
#include <aws/amplifyuibuilder/model/DeleteFormRequest.h>
#include <aws/amplifyuibuilder/model/DeleteThemeRequest.h>
#include <aws/amplifyuibuilder/model/ExchangeCodeForTokenRequest.h>
#include <aws/amplifyuibuilder/model/ExportComponentsRequest.h>
#include <aws/amplifyuibuilder/model/ExportFormsRequest.h>
#include <aws/amplifyuibuilder/model/ExportThemesRequest.h>
#include <aws/amplifyuibuilder/model/GetComponentRequest.h>
#include <aws/amplifyuibuilder/model/GetFormRequest.h>
#include <aws/amplifyuibuilder/model/GetMetadataRequest.h>
#include <aws/amplifyuibuilder/model/GetThemeRequest.h>
#include <aws/amplifyuibuilder/model/ListComponentsRequest.h>
#include <aws/amplifyuibuilder/model/ListFormsRequest.h>
#include <aws/amplifyuibuilder/model/ListThemesRequest.h>
#include <aws/amplifyuibuilder/model/PutMetadataFlagRequest.h>
#include <aws/amplifyuibuilder/model/RefreshTokenRequest.h>
#include <aws/amplifyuibuilder/model/UpdateComponentRequest.h>
#include <aws/amplifyuibuilder/model/UpdateFormRequest.h>
#include <aws/amplifyuibuilder/model/UpdateThemeRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::AmplifyUIBuilder;
using namespace Aws::AmplifyUIBuilder::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* AmplifyUIBuilderClient::SERVICE_NAME = "amplifyuibuilder";
const char* AmplifyUIBuilderClient::ALLOCATION_TAG = "AmplifyUIBuilderClient";

AmplifyUIBuilderClient::AmplifyUIBuilderClient(const AmplifyUIBuilder::AmplifyUIBuilderClientConfiguration& clientConfiguration,
                                               std::shared_ptr<AmplifyUIBuilderEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AmplifyUIBuilderErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

AmplifyUIBuilderClient::AmplifyUIBuilderClient(const AWSCredentials& credentials,
                                               std::shared_ptr<AmplifyUIBuilderEndpointProviderBase> endpointProvider,
                                               const AmplifyUIBuilder::AmplifyUIBuilderClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AmplifyUIBuilderErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

AmplifyUIBuilderClient::AmplifyUIBuilderClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               std::shared_ptr<AmplifyUIBuilderEndpointProviderBase> endpointProvider,
                                               const AmplifyUIBuilder::AmplifyUIBuilderClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AmplifyUIBuilderErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  AmplifyUIBuilderClient::AmplifyUIBuilderClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AmplifyUIBuilderErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<AmplifyUIBuilderEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

AmplifyUIBuilderClient::AmplifyUIBuilderClient(const AWSCredentials& credentials,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AmplifyUIBuilderErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<AmplifyUIBuilderEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

AmplifyUIBuilderClient::AmplifyUIBuilderClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                                               const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AmplifyUIBuilderErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<AmplifyUIBuilderEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
AmplifyUIBuilderClient::~AmplifyUIBuilderClient()
{
}

std::shared_ptr<AmplifyUIBuilderEndpointProviderBase>& AmplifyUIBuilderClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void AmplifyUIBuilderClient::init(const AmplifyUIBuilder::AmplifyUIBuilderClientConfiguration& config)
{
  AWSClient::SetServiceClientName("AmplifyUIBuilder");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void AmplifyUIBuilderClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

CreateComponentOutcome AmplifyUIBuilderClient::CreateComponent(const CreateComponentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateComponent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateComponent", "Required field: AppId, is not set");
    return CreateComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateComponent", "Required field: EnvironmentName, is not set");
    return CreateComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateComponent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/components");
  return CreateComponentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateComponentOutcomeCallable AmplifyUIBuilderClient::CreateComponentCallable(const CreateComponentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::CreateComponent, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::CreateComponentAsync(const CreateComponentRequest& request, const CreateComponentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::CreateComponent, this, request, handler, context, m_executor.get());
}

CreateFormOutcome AmplifyUIBuilderClient::CreateForm(const CreateFormRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateForm, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateForm", "Required field: AppId, is not set");
    return CreateFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateForm", "Required field: EnvironmentName, is not set");
    return CreateFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateForm, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/forms");
  return CreateFormOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateFormOutcomeCallable AmplifyUIBuilderClient::CreateFormCallable(const CreateFormRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::CreateForm, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::CreateFormAsync(const CreateFormRequest& request, const CreateFormResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::CreateForm, this, request, handler, context, m_executor.get());
}

CreateThemeOutcome AmplifyUIBuilderClient::CreateTheme(const CreateThemeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateTheme, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateTheme", "Required field: AppId, is not set");
    return CreateThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateTheme", "Required field: EnvironmentName, is not set");
    return CreateThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateTheme, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/themes");
  return CreateThemeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateThemeOutcomeCallable AmplifyUIBuilderClient::CreateThemeCallable(const CreateThemeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::CreateTheme, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::CreateThemeAsync(const CreateThemeRequest& request, const CreateThemeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::CreateTheme, this, request, handler, context, m_executor.get());
}

DeleteComponentOutcome AmplifyUIBuilderClient::DeleteComponent(const DeleteComponentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteComponent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteComponent", "Required field: AppId, is not set");
    return DeleteComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteComponent", "Required field: EnvironmentName, is not set");
    return DeleteComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteComponent", "Required field: Id, is not set");
    return DeleteComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteComponent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/components/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return DeleteComponentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteComponentOutcomeCallable AmplifyUIBuilderClient::DeleteComponentCallable(const DeleteComponentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::DeleteComponent, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::DeleteComponentAsync(const DeleteComponentRequest& request, const DeleteComponentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::DeleteComponent, this, request, handler, context, m_executor.get());
}

DeleteFormOutcome AmplifyUIBuilderClient::DeleteForm(const DeleteFormRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteForm, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteForm", "Required field: AppId, is not set");
    return DeleteFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteForm", "Required field: EnvironmentName, is not set");
    return DeleteFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteForm", "Required field: Id, is not set");
    return DeleteFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteForm, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/forms/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return DeleteFormOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteFormOutcomeCallable AmplifyUIBuilderClient::DeleteFormCallable(const DeleteFormRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::DeleteForm, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::DeleteFormAsync(const DeleteFormRequest& request, const DeleteFormResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::DeleteForm, this, request, handler, context, m_executor.get());
}

DeleteThemeOutcome AmplifyUIBuilderClient::DeleteTheme(const DeleteThemeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteTheme, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteTheme", "Required field: AppId, is not set");
    return DeleteThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteTheme", "Required field: EnvironmentName, is not set");
    return DeleteThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteTheme", "Required field: Id, is not set");
    return DeleteThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteTheme, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/themes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return DeleteThemeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteThemeOutcomeCallable AmplifyUIBuilderClient::DeleteThemeCallable(const DeleteThemeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::DeleteTheme, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::DeleteThemeAsync(const DeleteThemeRequest& request, const DeleteThemeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::DeleteTheme, this, request, handler, context, m_executor.get());
}

ExchangeCodeForTokenOutcome AmplifyUIBuilderClient::ExchangeCodeForToken(const ExchangeCodeForTokenRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ExchangeCodeForToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProviderHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExchangeCodeForToken", "Required field: Provider, is not set");
    return ExchangeCodeForTokenOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Provider]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ExchangeCodeForToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tokens/");
  endpointResolutionOutcome.GetResult().AddPathSegment(TokenProvidersMapper::GetNameForTokenProviders(request.GetProvider()));
  return ExchangeCodeForTokenOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

ExchangeCodeForTokenOutcomeCallable AmplifyUIBuilderClient::ExchangeCodeForTokenCallable(const ExchangeCodeForTokenRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::ExchangeCodeForToken, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::ExchangeCodeForTokenAsync(const ExchangeCodeForTokenRequest& request, const ExchangeCodeForTokenResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::ExchangeCodeForToken, this, request, handler, context, m_executor.get());
}

ExportComponentsOutcome AmplifyUIBuilderClient::ExportComponents(const ExportComponentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ExportComponents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExportComponents", "Required field: AppId, is not set");
    return ExportComponentsOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExportComponents", "Required field: EnvironmentName, is not set");
    return ExportComponentsOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ExportComponents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/export/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/components");
  return ExportComponentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ExportComponentsOutcomeCallable AmplifyUIBuilderClient::ExportComponentsCallable(const ExportComponentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::ExportComponents, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::ExportComponentsAsync(const ExportComponentsRequest& request, const ExportComponentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::ExportComponents, this, request, handler, context, m_executor.get());
}

ExportFormsOutcome AmplifyUIBuilderClient::ExportForms(const ExportFormsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ExportForms, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExportForms", "Required field: AppId, is not set");
    return ExportFormsOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExportForms", "Required field: EnvironmentName, is not set");
    return ExportFormsOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ExportForms, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/export/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/forms");
  return ExportFormsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ExportFormsOutcomeCallable AmplifyUIBuilderClient::ExportFormsCallable(const ExportFormsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::ExportForms, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::ExportFormsAsync(const ExportFormsRequest& request, const ExportFormsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::ExportForms, this, request, handler, context, m_executor.get());
}

ExportThemesOutcome AmplifyUIBuilderClient::ExportThemes(const ExportThemesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ExportThemes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExportThemes", "Required field: AppId, is not set");
    return ExportThemesOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ExportThemes", "Required field: EnvironmentName, is not set");
    return ExportThemesOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ExportThemes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/export/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/themes");
  return ExportThemesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ExportThemesOutcomeCallable AmplifyUIBuilderClient::ExportThemesCallable(const ExportThemesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::ExportThemes, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::ExportThemesAsync(const ExportThemesRequest& request, const ExportThemesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::ExportThemes, this, request, handler, context, m_executor.get());
}

GetComponentOutcome AmplifyUIBuilderClient::GetComponent(const GetComponentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetComponent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetComponent", "Required field: AppId, is not set");
    return GetComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetComponent", "Required field: EnvironmentName, is not set");
    return GetComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetComponent", "Required field: Id, is not set");
    return GetComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetComponent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/components/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return GetComponentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetComponentOutcomeCallable AmplifyUIBuilderClient::GetComponentCallable(const GetComponentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::GetComponent, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::GetComponentAsync(const GetComponentRequest& request, const GetComponentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::GetComponent, this, request, handler, context, m_executor.get());
}

GetFormOutcome AmplifyUIBuilderClient::GetForm(const GetFormRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetForm, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetForm", "Required field: AppId, is not set");
    return GetFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetForm", "Required field: EnvironmentName, is not set");
    return GetFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetForm", "Required field: Id, is not set");
    return GetFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetForm, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/forms/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return GetFormOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetFormOutcomeCallable AmplifyUIBuilderClient::GetFormCallable(const GetFormRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::GetForm, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::GetFormAsync(const GetFormRequest& request, const GetFormResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::GetForm, this, request, handler, context, m_executor.get());
}

GetMetadataOutcome AmplifyUIBuilderClient::GetMetadata(const GetMetadataRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetMetadata, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMetadata", "Required field: AppId, is not set");
    return GetMetadataOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetMetadata", "Required field: EnvironmentName, is not set");
    return GetMetadataOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetMetadata, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/metadata");
  return GetMetadataOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetMetadataOutcomeCallable AmplifyUIBuilderClient::GetMetadataCallable(const GetMetadataRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::GetMetadata, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::GetMetadataAsync(const GetMetadataRequest& request, const GetMetadataResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::GetMetadata, this, request, handler, context, m_executor.get());
}

GetThemeOutcome AmplifyUIBuilderClient::GetTheme(const GetThemeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetTheme, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTheme", "Required field: AppId, is not set");
    return GetThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTheme", "Required field: EnvironmentName, is not set");
    return GetThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetTheme", "Required field: Id, is not set");
    return GetThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetTheme, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/themes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return GetThemeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetThemeOutcomeCallable AmplifyUIBuilderClient::GetThemeCallable(const GetThemeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::GetTheme, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::GetThemeAsync(const GetThemeRequest& request, const GetThemeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::GetTheme, this, request, handler, context, m_executor.get());
}

ListComponentsOutcome AmplifyUIBuilderClient::ListComponents(const ListComponentsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListComponents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListComponents", "Required field: AppId, is not set");
    return ListComponentsOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListComponents", "Required field: EnvironmentName, is not set");
    return ListComponentsOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListComponents, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/components");
  return ListComponentsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListComponentsOutcomeCallable AmplifyUIBuilderClient::ListComponentsCallable(const ListComponentsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::ListComponents, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::ListComponentsAsync(const ListComponentsRequest& request, const ListComponentsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::ListComponents, this, request, handler, context, m_executor.get());
}

ListFormsOutcome AmplifyUIBuilderClient::ListForms(const ListFormsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListForms, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListForms", "Required field: AppId, is not set");
    return ListFormsOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListForms", "Required field: EnvironmentName, is not set");
    return ListFormsOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListForms, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/forms");
  return ListFormsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListFormsOutcomeCallable AmplifyUIBuilderClient::ListFormsCallable(const ListFormsRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::ListForms, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::ListFormsAsync(const ListFormsRequest& request, const ListFormsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::ListForms, this, request, handler, context, m_executor.get());
}

ListThemesOutcome AmplifyUIBuilderClient::ListThemes(const ListThemesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListThemes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListThemes", "Required field: AppId, is not set");
    return ListThemesOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListThemes", "Required field: EnvironmentName, is not set");
    return ListThemesOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListThemes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/themes");
  return ListThemesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListThemesOutcomeCallable AmplifyUIBuilderClient::ListThemesCallable(const ListThemesRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::ListThemes, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::ListThemesAsync(const ListThemesRequest& request, const ListThemesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::ListThemes, this, request, handler, context, m_executor.get());
}

PutMetadataFlagOutcome AmplifyUIBuilderClient::PutMetadataFlag(const PutMetadataFlagRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, PutMetadataFlag, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMetadataFlag", "Required field: AppId, is not set");
    return PutMetadataFlagOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMetadataFlag", "Required field: EnvironmentName, is not set");
    return PutMetadataFlagOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.FeatureNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("PutMetadataFlag", "Required field: FeatureName, is not set");
    return PutMetadataFlagOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FeatureName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, PutMetadataFlag, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/metadata/features/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFeatureName());
  return PutMetadataFlagOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PUT, Aws::Auth::SIGV4_SIGNER));
}

PutMetadataFlagOutcomeCallable AmplifyUIBuilderClient::PutMetadataFlagCallable(const PutMetadataFlagRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::PutMetadataFlag, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::PutMetadataFlagAsync(const PutMetadataFlagRequest& request, const PutMetadataFlagResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::PutMetadataFlag, this, request, handler, context, m_executor.get());
}

RefreshTokenOutcome AmplifyUIBuilderClient::RefreshToken(const RefreshTokenRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, RefreshToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ProviderHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("RefreshToken", "Required field: Provider, is not set");
    return RefreshTokenOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Provider]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, RefreshToken, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/tokens/");
  endpointResolutionOutcome.GetResult().AddPathSegment(TokenProvidersMapper::GetNameForTokenProviders(request.GetProvider()));
  endpointResolutionOutcome.GetResult().AddPathSegments("/refresh");
  return RefreshTokenOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

RefreshTokenOutcomeCallable AmplifyUIBuilderClient::RefreshTokenCallable(const RefreshTokenRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::RefreshToken, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::RefreshTokenAsync(const RefreshTokenRequest& request, const RefreshTokenResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::RefreshToken, this, request, handler, context, m_executor.get());
}

UpdateComponentOutcome AmplifyUIBuilderClient::UpdateComponent(const UpdateComponentRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateComponent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateComponent", "Required field: AppId, is not set");
    return UpdateComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateComponent", "Required field: EnvironmentName, is not set");
    return UpdateComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateComponent", "Required field: Id, is not set");
    return UpdateComponentOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateComponent, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/components/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return UpdateComponentOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateComponentOutcomeCallable AmplifyUIBuilderClient::UpdateComponentCallable(const UpdateComponentRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::UpdateComponent, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::UpdateComponentAsync(const UpdateComponentRequest& request, const UpdateComponentResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::UpdateComponent, this, request, handler, context, m_executor.get());
}

UpdateFormOutcome AmplifyUIBuilderClient::UpdateForm(const UpdateFormRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateForm, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateForm", "Required field: AppId, is not set");
    return UpdateFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateForm", "Required field: EnvironmentName, is not set");
    return UpdateFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateForm", "Required field: Id, is not set");
    return UpdateFormOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateForm, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/forms/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return UpdateFormOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateFormOutcomeCallable AmplifyUIBuilderClient::UpdateFormCallable(const UpdateFormRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::UpdateForm, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::UpdateFormAsync(const UpdateFormRequest& request, const UpdateFormResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::UpdateForm, this, request, handler, context, m_executor.get());
}

UpdateThemeOutcome AmplifyUIBuilderClient::UpdateTheme(const UpdateThemeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateTheme, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.AppIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateTheme", "Required field: AppId, is not set");
    return UpdateThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [AppId]", false));
  }
  if (!request.EnvironmentNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateTheme", "Required field: EnvironmentName, is not set");
    return UpdateThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [EnvironmentName]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateTheme", "Required field: Id, is not set");
    return UpdateThemeOutcome(Aws::Client::AWSError<AmplifyUIBuilderErrors>(AmplifyUIBuilderErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateTheme, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/app/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetAppId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/environment/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetEnvironmentName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/themes/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return UpdateThemeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_PATCH, Aws::Auth::SIGV4_SIGNER));
}

UpdateThemeOutcomeCallable AmplifyUIBuilderClient::UpdateThemeCallable(const UpdateThemeRequest& request) const
{
  return MakeCallableOperation(ALLOCATION_TAG, &AmplifyUIBuilderClient::UpdateTheme, this, request, m_executor.get());
}

void AmplifyUIBuilderClient::UpdateThemeAsync(const UpdateThemeRequest& request, const UpdateThemeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  MakeAsyncOperation(&AmplifyUIBuilderClient::UpdateTheme, this, request, handler, context, m_executor.get());
}

