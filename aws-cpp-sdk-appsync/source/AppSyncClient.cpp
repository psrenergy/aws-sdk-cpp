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

#include <aws/appsync/AppSyncClient.h>
#include <aws/appsync/AppSyncErrorMarshaller.h>
#include <aws/appsync/AppSyncEndpointProvider.h>
#include <aws/appsync/model/AssociateApiRequest.h>
#include <aws/appsync/model/CreateApiCacheRequest.h>
#include <aws/appsync/model/CreateApiKeyRequest.h>
#include <aws/appsync/model/CreateDataSourceRequest.h>
#include <aws/appsync/model/CreateDomainNameRequest.h>
#include <aws/appsync/model/CreateFunctionRequest.h>
#include <aws/appsync/model/CreateGraphqlApiRequest.h>
#include <aws/appsync/model/CreateResolverRequest.h>
#include <aws/appsync/model/CreateTypeRequest.h>
#include <aws/appsync/model/DeleteApiCacheRequest.h>
#include <aws/appsync/model/DeleteApiKeyRequest.h>
#include <aws/appsync/model/DeleteDataSourceRequest.h>
#include <aws/appsync/model/DeleteDomainNameRequest.h>
#include <aws/appsync/model/DeleteFunctionRequest.h>
#include <aws/appsync/model/DeleteGraphqlApiRequest.h>
#include <aws/appsync/model/DeleteResolverRequest.h>
#include <aws/appsync/model/DeleteTypeRequest.h>
#include <aws/appsync/model/DisassociateApiRequest.h>
#include <aws/appsync/model/EvaluateCodeRequest.h>
#include <aws/appsync/model/EvaluateMappingTemplateRequest.h>
#include <aws/appsync/model/FlushApiCacheRequest.h>
#include <aws/appsync/model/GetApiAssociationRequest.h>
#include <aws/appsync/model/GetApiCacheRequest.h>
#include <aws/appsync/model/GetDataSourceRequest.h>
#include <aws/appsync/model/GetDomainNameRequest.h>
#include <aws/appsync/model/GetFunctionRequest.h>
#include <aws/appsync/model/GetGraphqlApiRequest.h>
#include <aws/appsync/model/GetIntrospectionSchemaRequest.h>
#include <aws/appsync/model/GetResolverRequest.h>
#include <aws/appsync/model/GetSchemaCreationStatusRequest.h>
#include <aws/appsync/model/GetTypeRequest.h>
#include <aws/appsync/model/ListApiKeysRequest.h>
#include <aws/appsync/model/ListDataSourcesRequest.h>
#include <aws/appsync/model/ListDomainNamesRequest.h>
#include <aws/appsync/model/ListFunctionsRequest.h>
#include <aws/appsync/model/ListGraphqlApisRequest.h>
#include <aws/appsync/model/ListResolversRequest.h>
#include <aws/appsync/model/ListResolversByFunctionRequest.h>
#include <aws/appsync/model/ListTagsForResourceRequest.h>
#include <aws/appsync/model/ListTypesRequest.h>
#include <aws/appsync/model/StartSchemaCreationRequest.h>
#include <aws/appsync/model/TagResourceRequest.h>
#include <aws/appsync/model/UntagResourceRequest.h>
#include <aws/appsync/model/UpdateApiCacheRequest.h>
#include <aws/appsync/model/UpdateApiKeyRequest.h>
#include <aws/appsync/model/UpdateDataSourceRequest.h>
#include <aws/appsync/model/UpdateDomainNameRequest.h>
#include <aws/appsync/model/UpdateFunctionRequest.h>
#include <aws/appsync/model/UpdateGraphqlApiRequest.h>
#include <aws/appsync/model/UpdateResolverRequest.h>
#include <aws/appsync/model/UpdateTypeRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::AppSync;
using namespace Aws::AppSync::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;
using ResolveEndpointOutcome = Aws::Endpoint::ResolveEndpointOutcome;

const char* AppSyncClient::SERVICE_NAME = "appsync";
const char* AppSyncClient::ALLOCATION_TAG = "AppSyncClient";

AppSyncClient::AppSyncClient(const AppSync::AppSyncClientConfiguration& clientConfiguration,
                             std::shared_ptr<AppSyncEndpointProviderBase> endpointProvider) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppSyncErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

AppSyncClient::AppSyncClient(const AWSCredentials& credentials,
                             std::shared_ptr<AppSyncEndpointProviderBase> endpointProvider,
                             const AppSync::AppSyncClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppSyncErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

AppSyncClient::AppSyncClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                             std::shared_ptr<AppSyncEndpointProviderBase> endpointProvider,
                             const AppSync::AppSyncClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppSyncErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(std::move(endpointProvider))
{
  init(m_clientConfiguration);
}

    /* Legacy constructors due deprecation */
  AppSyncClient::AppSyncClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppSyncErrorMarshaller>(ALLOCATION_TAG)),
  m_clientConfiguration(clientConfiguration),
  m_executor(clientConfiguration.executor),
  m_endpointProvider(Aws::MakeShared<AppSyncEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

AppSyncClient::AppSyncClient(const AWSCredentials& credentials,
                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppSyncErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<AppSyncEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

AppSyncClient::AppSyncClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
                             const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(clientConfiguration,
            Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG,
                                             credentialsProvider,
                                             SERVICE_NAME,
                                             Aws::Region::ComputeSignerRegion(clientConfiguration.region)),
            Aws::MakeShared<AppSyncErrorMarshaller>(ALLOCATION_TAG)),
    m_clientConfiguration(clientConfiguration),
    m_executor(clientConfiguration.executor),
    m_endpointProvider(Aws::MakeShared<AppSyncEndpointProvider>(ALLOCATION_TAG))
{
  init(m_clientConfiguration);
}

    /* End of legacy constructors due deprecation */
AppSyncClient::~AppSyncClient()
{
}

std::shared_ptr<AppSyncEndpointProviderBase>& AppSyncClient::accessEndpointProvider()
{
  return m_endpointProvider;
}

void AppSyncClient::init(const AppSync::AppSyncClientConfiguration& config)
{
  AWSClient::SetServiceClientName("AppSync");
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->InitBuiltInParameters(config);
}

void AppSyncClient::OverrideEndpoint(const Aws::String& endpoint)
{
  AWS_CHECK_PTR(SERVICE_NAME, m_endpointProvider);
  m_endpointProvider->OverrideEndpoint(endpoint);
}

AssociateApiOutcome AppSyncClient::AssociateApi(const AssociateApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, AssociateApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("AssociateApi", "Required field: DomainName, is not set");
    return AssociateApiOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, AssociateApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apiassociation");
  return AssociateApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

AssociateApiOutcomeCallable AppSyncClient::AssociateApiCallable(const AssociateApiRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(AssociateApi, request, m_executor.get());
}

void AppSyncClient::AssociateApiAsync(const AssociateApiRequest& request, const AssociateApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(AssociateApi, request, handler, context, m_executor.get());
}

CreateApiCacheOutcome AppSyncClient::CreateApiCache(const CreateApiCacheRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateApiCache", "Required field: ApiId, is not set");
    return CreateApiCacheOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ApiCaches");
  return CreateApiCacheOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateApiCacheOutcomeCallable AppSyncClient::CreateApiCacheCallable(const CreateApiCacheRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateApiCache, request, m_executor.get());
}

void AppSyncClient::CreateApiCacheAsync(const CreateApiCacheRequest& request, const CreateApiCacheResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateApiCache, request, handler, context, m_executor.get());
}

CreateApiKeyOutcome AppSyncClient::CreateApiKey(const CreateApiKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateApiKey", "Required field: ApiId, is not set");
    return CreateApiKeyOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys");
  return CreateApiKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateApiKeyOutcomeCallable AppSyncClient::CreateApiKeyCallable(const CreateApiKeyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateApiKey, request, m_executor.get());
}

void AppSyncClient::CreateApiKeyAsync(const CreateApiKeyRequest& request, const CreateApiKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateApiKey, request, handler, context, m_executor.get());
}

CreateDataSourceOutcome AppSyncClient::CreateDataSource(const CreateDataSourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDataSource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateDataSource", "Required field: ApiId, is not set");
    return CreateDataSourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDataSource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasources");
  return CreateDataSourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateDataSourceOutcomeCallable AppSyncClient::CreateDataSourceCallable(const CreateDataSourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateDataSource, request, m_executor.get());
}

void AppSyncClient::CreateDataSourceAsync(const CreateDataSourceRequest& request, const CreateDataSourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateDataSource, request, handler, context, m_executor.get());
}

CreateDomainNameOutcome AppSyncClient::CreateDomainName(const CreateDomainNameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/domainnames");
  return CreateDomainNameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateDomainNameOutcomeCallable AppSyncClient::CreateDomainNameCallable(const CreateDomainNameRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateDomainName, request, m_executor.get());
}

void AppSyncClient::CreateDomainNameAsync(const CreateDomainNameRequest& request, const CreateDomainNameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateDomainName, request, handler, context, m_executor.get());
}

CreateFunctionOutcome AppSyncClient::CreateFunction(const CreateFunctionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateFunction", "Required field: ApiId, is not set");
    return CreateFunctionOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/functions");
  return CreateFunctionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateFunctionOutcomeCallable AppSyncClient::CreateFunctionCallable(const CreateFunctionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateFunction, request, m_executor.get());
}

void AppSyncClient::CreateFunctionAsync(const CreateFunctionRequest& request, const CreateFunctionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateFunction, request, handler, context, m_executor.get());
}

CreateGraphqlApiOutcome AppSyncClient::CreateGraphqlApi(const CreateGraphqlApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateGraphqlApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateGraphqlApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis");
  return CreateGraphqlApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateGraphqlApiOutcomeCallable AppSyncClient::CreateGraphqlApiCallable(const CreateGraphqlApiRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateGraphqlApi, request, m_executor.get());
}

void AppSyncClient::CreateGraphqlApiAsync(const CreateGraphqlApiRequest& request, const CreateGraphqlApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateGraphqlApi, request, handler, context, m_executor.get());
}

CreateResolverOutcome AppSyncClient::CreateResolver(const CreateResolverRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateResolver, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateResolver", "Required field: ApiId, is not set");
    return CreateResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.TypeNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateResolver", "Required field: TypeName, is not set");
    return CreateResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TypeName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateResolver, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTypeName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resolvers");
  return CreateResolverOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateResolverOutcomeCallable AppSyncClient::CreateResolverCallable(const CreateResolverRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateResolver, request, m_executor.get());
}

void AppSyncClient::CreateResolverAsync(const CreateResolverRequest& request, const CreateResolverResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateResolver, request, handler, context, m_executor.get());
}

CreateTypeOutcome AppSyncClient::CreateType(const CreateTypeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, CreateType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("CreateType", "Required field: ApiId, is not set");
    return CreateTypeOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, CreateType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types");
  return CreateTypeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

CreateTypeOutcomeCallable AppSyncClient::CreateTypeCallable(const CreateTypeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(CreateType, request, m_executor.get());
}

void AppSyncClient::CreateTypeAsync(const CreateTypeRequest& request, const CreateTypeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(CreateType, request, handler, context, m_executor.get());
}

DeleteApiCacheOutcome AppSyncClient::DeleteApiCache(const DeleteApiCacheRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteApiCache", "Required field: ApiId, is not set");
    return DeleteApiCacheOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ApiCaches");
  return DeleteApiCacheOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteApiCacheOutcomeCallable AppSyncClient::DeleteApiCacheCallable(const DeleteApiCacheRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteApiCache, request, m_executor.get());
}

void AppSyncClient::DeleteApiCacheAsync(const DeleteApiCacheRequest& request, const DeleteApiCacheResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteApiCache, request, handler, context, m_executor.get());
}

DeleteApiKeyOutcome AppSyncClient::DeleteApiKey(const DeleteApiKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteApiKey", "Required field: ApiId, is not set");
    return DeleteApiKeyOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteApiKey", "Required field: Id, is not set");
    return DeleteApiKeyOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return DeleteApiKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteApiKeyOutcomeCallable AppSyncClient::DeleteApiKeyCallable(const DeleteApiKeyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteApiKey, request, m_executor.get());
}

void AppSyncClient::DeleteApiKeyAsync(const DeleteApiKeyRequest& request, const DeleteApiKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteApiKey, request, handler, context, m_executor.get());
}

DeleteDataSourceOutcome AppSyncClient::DeleteDataSource(const DeleteDataSourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDataSource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDataSource", "Required field: ApiId, is not set");
    return DeleteDataSourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDataSource", "Required field: Name, is not set");
    return DeleteDataSourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDataSource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return DeleteDataSourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteDataSourceOutcomeCallable AppSyncClient::DeleteDataSourceCallable(const DeleteDataSourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteDataSource, request, m_executor.get());
}

void AppSyncClient::DeleteDataSourceAsync(const DeleteDataSourceRequest& request, const DeleteDataSourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteDataSource, request, handler, context, m_executor.get());
}

DeleteDomainNameOutcome AppSyncClient::DeleteDomainName(const DeleteDomainNameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteDomainName", "Required field: DomainName, is not set");
    return DeleteDomainNameOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  return DeleteDomainNameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteDomainNameOutcomeCallable AppSyncClient::DeleteDomainNameCallable(const DeleteDomainNameRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteDomainName, request, m_executor.get());
}

void AppSyncClient::DeleteDomainNameAsync(const DeleteDomainNameRequest& request, const DeleteDomainNameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteDomainName, request, handler, context, m_executor.get());
}

DeleteFunctionOutcome AppSyncClient::DeleteFunction(const DeleteFunctionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteFunction", "Required field: ApiId, is not set");
    return DeleteFunctionOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.FunctionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteFunction", "Required field: FunctionId, is not set");
    return DeleteFunctionOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FunctionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/functions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFunctionId());
  return DeleteFunctionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteFunctionOutcomeCallable AppSyncClient::DeleteFunctionCallable(const DeleteFunctionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteFunction, request, m_executor.get());
}

void AppSyncClient::DeleteFunctionAsync(const DeleteFunctionRequest& request, const DeleteFunctionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteFunction, request, handler, context, m_executor.get());
}

DeleteGraphqlApiOutcome AppSyncClient::DeleteGraphqlApi(const DeleteGraphqlApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteGraphqlApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteGraphqlApi", "Required field: ApiId, is not set");
    return DeleteGraphqlApiOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteGraphqlApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  return DeleteGraphqlApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteGraphqlApiOutcomeCallable AppSyncClient::DeleteGraphqlApiCallable(const DeleteGraphqlApiRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteGraphqlApi, request, m_executor.get());
}

void AppSyncClient::DeleteGraphqlApiAsync(const DeleteGraphqlApiRequest& request, const DeleteGraphqlApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteGraphqlApi, request, handler, context, m_executor.get());
}

DeleteResolverOutcome AppSyncClient::DeleteResolver(const DeleteResolverRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteResolver, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteResolver", "Required field: ApiId, is not set");
    return DeleteResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.TypeNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteResolver", "Required field: TypeName, is not set");
    return DeleteResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TypeName]", false));
  }
  if (!request.FieldNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteResolver", "Required field: FieldName, is not set");
    return DeleteResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FieldName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteResolver, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTypeName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resolvers/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFieldName());
  return DeleteResolverOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteResolverOutcomeCallable AppSyncClient::DeleteResolverCallable(const DeleteResolverRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteResolver, request, m_executor.get());
}

void AppSyncClient::DeleteResolverAsync(const DeleteResolverRequest& request, const DeleteResolverResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteResolver, request, handler, context, m_executor.get());
}

DeleteTypeOutcome AppSyncClient::DeleteType(const DeleteTypeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DeleteType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteType", "Required field: ApiId, is not set");
    return DeleteTypeOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.TypeNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DeleteType", "Required field: TypeName, is not set");
    return DeleteTypeOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TypeName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DeleteType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTypeName());
  return DeleteTypeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DeleteTypeOutcomeCallable AppSyncClient::DeleteTypeCallable(const DeleteTypeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DeleteType, request, m_executor.get());
}

void AppSyncClient::DeleteTypeAsync(const DeleteTypeRequest& request, const DeleteTypeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DeleteType, request, handler, context, m_executor.get());
}

DisassociateApiOutcome AppSyncClient::DisassociateApi(const DisassociateApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, DisassociateApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("DisassociateApi", "Required field: DomainName, is not set");
    return DisassociateApiOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, DisassociateApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apiassociation");
  return DisassociateApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

DisassociateApiOutcomeCallable AppSyncClient::DisassociateApiCallable(const DisassociateApiRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(DisassociateApi, request, m_executor.get());
}

void AppSyncClient::DisassociateApiAsync(const DisassociateApiRequest& request, const DisassociateApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(DisassociateApi, request, handler, context, m_executor.get());
}

EvaluateCodeOutcome AppSyncClient::EvaluateCode(const EvaluateCodeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, EvaluateCode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, EvaluateCode, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/dataplane-evaluatecode");
  return EvaluateCodeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

EvaluateCodeOutcomeCallable AppSyncClient::EvaluateCodeCallable(const EvaluateCodeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(EvaluateCode, request, m_executor.get());
}

void AppSyncClient::EvaluateCodeAsync(const EvaluateCodeRequest& request, const EvaluateCodeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(EvaluateCode, request, handler, context, m_executor.get());
}

EvaluateMappingTemplateOutcome AppSyncClient::EvaluateMappingTemplate(const EvaluateMappingTemplateRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, EvaluateMappingTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, EvaluateMappingTemplate, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/dataplane-evaluatetemplate");
  return EvaluateMappingTemplateOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

EvaluateMappingTemplateOutcomeCallable AppSyncClient::EvaluateMappingTemplateCallable(const EvaluateMappingTemplateRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(EvaluateMappingTemplate, request, m_executor.get());
}

void AppSyncClient::EvaluateMappingTemplateAsync(const EvaluateMappingTemplateRequest& request, const EvaluateMappingTemplateResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(EvaluateMappingTemplate, request, handler, context, m_executor.get());
}

FlushApiCacheOutcome AppSyncClient::FlushApiCache(const FlushApiCacheRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, FlushApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("FlushApiCache", "Required field: ApiId, is not set");
    return FlushApiCacheOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, FlushApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/FlushCache");
  return FlushApiCacheOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

FlushApiCacheOutcomeCallable AppSyncClient::FlushApiCacheCallable(const FlushApiCacheRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(FlushApiCache, request, m_executor.get());
}

void AppSyncClient::FlushApiCacheAsync(const FlushApiCacheRequest& request, const FlushApiCacheResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(FlushApiCache, request, handler, context, m_executor.get());
}

GetApiAssociationOutcome AppSyncClient::GetApiAssociation(const GetApiAssociationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetApiAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetApiAssociation", "Required field: DomainName, is not set");
    return GetApiAssociationOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetApiAssociation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apiassociation");
  return GetApiAssociationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetApiAssociationOutcomeCallable AppSyncClient::GetApiAssociationCallable(const GetApiAssociationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetApiAssociation, request, m_executor.get());
}

void AppSyncClient::GetApiAssociationAsync(const GetApiAssociationRequest& request, const GetApiAssociationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetApiAssociation, request, handler, context, m_executor.get());
}

GetApiCacheOutcome AppSyncClient::GetApiCache(const GetApiCacheRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetApiCache", "Required field: ApiId, is not set");
    return GetApiCacheOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ApiCaches");
  return GetApiCacheOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetApiCacheOutcomeCallable AppSyncClient::GetApiCacheCallable(const GetApiCacheRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetApiCache, request, m_executor.get());
}

void AppSyncClient::GetApiCacheAsync(const GetApiCacheRequest& request, const GetApiCacheResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetApiCache, request, handler, context, m_executor.get());
}

GetDataSourceOutcome AppSyncClient::GetDataSource(const GetDataSourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDataSource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDataSource", "Required field: ApiId, is not set");
    return GetDataSourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDataSource", "Required field: Name, is not set");
    return GetDataSourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDataSource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return GetDataSourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDataSourceOutcomeCallable AppSyncClient::GetDataSourceCallable(const GetDataSourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetDataSource, request, m_executor.get());
}

void AppSyncClient::GetDataSourceAsync(const GetDataSourceRequest& request, const GetDataSourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetDataSource, request, handler, context, m_executor.get());
}

GetDomainNameOutcome AppSyncClient::GetDomainName(const GetDomainNameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetDomainName", "Required field: DomainName, is not set");
    return GetDomainNameOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  return GetDomainNameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetDomainNameOutcomeCallable AppSyncClient::GetDomainNameCallable(const GetDomainNameRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetDomainName, request, m_executor.get());
}

void AppSyncClient::GetDomainNameAsync(const GetDomainNameRequest& request, const GetDomainNameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetDomainName, request, handler, context, m_executor.get());
}

GetFunctionOutcome AppSyncClient::GetFunction(const GetFunctionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetFunction", "Required field: ApiId, is not set");
    return GetFunctionOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.FunctionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetFunction", "Required field: FunctionId, is not set");
    return GetFunctionOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FunctionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/functions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFunctionId());
  return GetFunctionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetFunctionOutcomeCallable AppSyncClient::GetFunctionCallable(const GetFunctionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetFunction, request, m_executor.get());
}

void AppSyncClient::GetFunctionAsync(const GetFunctionRequest& request, const GetFunctionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetFunction, request, handler, context, m_executor.get());
}

GetGraphqlApiOutcome AppSyncClient::GetGraphqlApi(const GetGraphqlApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetGraphqlApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetGraphqlApi", "Required field: ApiId, is not set");
    return GetGraphqlApiOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetGraphqlApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  return GetGraphqlApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetGraphqlApiOutcomeCallable AppSyncClient::GetGraphqlApiCallable(const GetGraphqlApiRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetGraphqlApi, request, m_executor.get());
}

void AppSyncClient::GetGraphqlApiAsync(const GetGraphqlApiRequest& request, const GetGraphqlApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetGraphqlApi, request, handler, context, m_executor.get());
}

GetIntrospectionSchemaOutcome AppSyncClient::GetIntrospectionSchema(const GetIntrospectionSchemaRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetIntrospectionSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntrospectionSchema", "Required field: ApiId, is not set");
    return GetIntrospectionSchemaOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.FormatHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetIntrospectionSchema", "Required field: Format, is not set");
    return GetIntrospectionSchemaOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Format]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetIntrospectionSchema, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/schema");
  return GetIntrospectionSchemaOutcome(MakeRequestWithUnparsedResponse(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET));
}

GetIntrospectionSchemaOutcomeCallable AppSyncClient::GetIntrospectionSchemaCallable(const GetIntrospectionSchemaRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetIntrospectionSchema, request, m_executor.get());
}

void AppSyncClient::GetIntrospectionSchemaAsync(const GetIntrospectionSchemaRequest& request, const GetIntrospectionSchemaResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetIntrospectionSchema, request, handler, context, m_executor.get());
}

GetResolverOutcome AppSyncClient::GetResolver(const GetResolverRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetResolver, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetResolver", "Required field: ApiId, is not set");
    return GetResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.TypeNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetResolver", "Required field: TypeName, is not set");
    return GetResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TypeName]", false));
  }
  if (!request.FieldNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetResolver", "Required field: FieldName, is not set");
    return GetResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FieldName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetResolver, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTypeName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resolvers/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFieldName());
  return GetResolverOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetResolverOutcomeCallable AppSyncClient::GetResolverCallable(const GetResolverRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetResolver, request, m_executor.get());
}

void AppSyncClient::GetResolverAsync(const GetResolverRequest& request, const GetResolverResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetResolver, request, handler, context, m_executor.get());
}

GetSchemaCreationStatusOutcome AppSyncClient::GetSchemaCreationStatus(const GetSchemaCreationStatusRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetSchemaCreationStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetSchemaCreationStatus", "Required field: ApiId, is not set");
    return GetSchemaCreationStatusOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetSchemaCreationStatus, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/schemacreation");
  return GetSchemaCreationStatusOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetSchemaCreationStatusOutcomeCallable AppSyncClient::GetSchemaCreationStatusCallable(const GetSchemaCreationStatusRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetSchemaCreationStatus, request, m_executor.get());
}

void AppSyncClient::GetSchemaCreationStatusAsync(const GetSchemaCreationStatusRequest& request, const GetSchemaCreationStatusResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetSchemaCreationStatus, request, handler, context, m_executor.get());
}

GetTypeOutcome AppSyncClient::GetType(const GetTypeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, GetType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetType", "Required field: ApiId, is not set");
    return GetTypeOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.TypeNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetType", "Required field: TypeName, is not set");
    return GetTypeOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TypeName]", false));
  }
  if (!request.FormatHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("GetType", "Required field: Format, is not set");
    return GetTypeOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Format]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, GetType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTypeName());
  return GetTypeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

GetTypeOutcomeCallable AppSyncClient::GetTypeCallable(const GetTypeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(GetType, request, m_executor.get());
}

void AppSyncClient::GetTypeAsync(const GetTypeRequest& request, const GetTypeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(GetType, request, handler, context, m_executor.get());
}

ListApiKeysOutcome AppSyncClient::ListApiKeys(const ListApiKeysRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListApiKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListApiKeys", "Required field: ApiId, is not set");
    return ListApiKeysOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListApiKeys, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys");
  return ListApiKeysOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListApiKeysOutcomeCallable AppSyncClient::ListApiKeysCallable(const ListApiKeysRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListApiKeys, request, m_executor.get());
}

void AppSyncClient::ListApiKeysAsync(const ListApiKeysRequest& request, const ListApiKeysResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListApiKeys, request, handler, context, m_executor.get());
}

ListDataSourcesOutcome AppSyncClient::ListDataSources(const ListDataSourcesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDataSources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListDataSources", "Required field: ApiId, is not set");
    return ListDataSourcesOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDataSources, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasources");
  return ListDataSourcesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListDataSourcesOutcomeCallable AppSyncClient::ListDataSourcesCallable(const ListDataSourcesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListDataSources, request, m_executor.get());
}

void AppSyncClient::ListDataSourcesAsync(const ListDataSourcesRequest& request, const ListDataSourcesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListDataSources, request, handler, context, m_executor.get());
}

ListDomainNamesOutcome AppSyncClient::ListDomainNames(const ListDomainNamesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListDomainNames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListDomainNames, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/domainnames");
  return ListDomainNamesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListDomainNamesOutcomeCallable AppSyncClient::ListDomainNamesCallable(const ListDomainNamesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListDomainNames, request, m_executor.get());
}

void AppSyncClient::ListDomainNamesAsync(const ListDomainNamesRequest& request, const ListDomainNamesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListDomainNames, request, handler, context, m_executor.get());
}

ListFunctionsOutcome AppSyncClient::ListFunctions(const ListFunctionsRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListFunctions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListFunctions", "Required field: ApiId, is not set");
    return ListFunctionsOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListFunctions, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/functions");
  return ListFunctionsOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListFunctionsOutcomeCallable AppSyncClient::ListFunctionsCallable(const ListFunctionsRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListFunctions, request, m_executor.get());
}

void AppSyncClient::ListFunctionsAsync(const ListFunctionsRequest& request, const ListFunctionsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListFunctions, request, handler, context, m_executor.get());
}

ListGraphqlApisOutcome AppSyncClient::ListGraphqlApis(const ListGraphqlApisRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListGraphqlApis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListGraphqlApis, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis");
  return ListGraphqlApisOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListGraphqlApisOutcomeCallable AppSyncClient::ListGraphqlApisCallable(const ListGraphqlApisRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListGraphqlApis, request, m_executor.get());
}

void AppSyncClient::ListGraphqlApisAsync(const ListGraphqlApisRequest& request, const ListGraphqlApisResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListGraphqlApis, request, handler, context, m_executor.get());
}

ListResolversOutcome AppSyncClient::ListResolvers(const ListResolversRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListResolvers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListResolvers", "Required field: ApiId, is not set");
    return ListResolversOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.TypeNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListResolvers", "Required field: TypeName, is not set");
    return ListResolversOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TypeName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListResolvers, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTypeName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resolvers");
  return ListResolversOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListResolversOutcomeCallable AppSyncClient::ListResolversCallable(const ListResolversRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListResolvers, request, m_executor.get());
}

void AppSyncClient::ListResolversAsync(const ListResolversRequest& request, const ListResolversResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListResolvers, request, handler, context, m_executor.get());
}

ListResolversByFunctionOutcome AppSyncClient::ListResolversByFunction(const ListResolversByFunctionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListResolversByFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListResolversByFunction", "Required field: ApiId, is not set");
    return ListResolversByFunctionOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.FunctionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListResolversByFunction", "Required field: FunctionId, is not set");
    return ListResolversByFunctionOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FunctionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListResolversByFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/functions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFunctionId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resolvers");
  return ListResolversByFunctionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListResolversByFunctionOutcomeCallable AppSyncClient::ListResolversByFunctionCallable(const ListResolversByFunctionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListResolversByFunction, request, m_executor.get());
}

void AppSyncClient::ListResolversByFunctionAsync(const ListResolversByFunctionRequest& request, const ListResolversByFunctionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListResolversByFunction, request, handler, context, m_executor.get());
}

ListTagsForResourceOutcome AppSyncClient::ListTagsForResource(const ListTagsForResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTagsForResource", "Required field: ResourceArn, is not set");
    return ListTagsForResourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTagsForResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return ListTagsForResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTagsForResourceOutcomeCallable AppSyncClient::ListTagsForResourceCallable(const ListTagsForResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTagsForResource, request, m_executor.get());
}

void AppSyncClient::ListTagsForResourceAsync(const ListTagsForResourceRequest& request, const ListTagsForResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTagsForResource, request, handler, context, m_executor.get());
}

ListTypesOutcome AppSyncClient::ListTypes(const ListTypesRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, ListTypes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTypes", "Required field: ApiId, is not set");
    return ListTypesOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.FormatHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("ListTypes", "Required field: Format, is not set");
    return ListTypesOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Format]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, ListTypes, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types");
  return ListTypesOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_GET, Aws::Auth::SIGV4_SIGNER));
}

ListTypesOutcomeCallable AppSyncClient::ListTypesCallable(const ListTypesRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(ListTypes, request, m_executor.get());
}

void AppSyncClient::ListTypesAsync(const ListTypesRequest& request, const ListTypesResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(ListTypes, request, handler, context, m_executor.get());
}

StartSchemaCreationOutcome AppSyncClient::StartSchemaCreation(const StartSchemaCreationRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, StartSchemaCreation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("StartSchemaCreation", "Required field: ApiId, is not set");
    return StartSchemaCreationOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, StartSchemaCreation, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/schemacreation");
  return StartSchemaCreationOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

StartSchemaCreationOutcomeCallable AppSyncClient::StartSchemaCreationCallable(const StartSchemaCreationRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(StartSchemaCreation, request, m_executor.get());
}

void AppSyncClient::StartSchemaCreationAsync(const StartSchemaCreationRequest& request, const StartSchemaCreationResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(StartSchemaCreation, request, handler, context, m_executor.get());
}

TagResourceOutcome AppSyncClient::TagResource(const TagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("TagResource", "Required field: ResourceArn, is not set");
    return TagResourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, TagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return TagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

TagResourceOutcomeCallable AppSyncClient::TagResourceCallable(const TagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(TagResource, request, m_executor.get());
}

void AppSyncClient::TagResourceAsync(const TagResourceRequest& request, const TagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(TagResource, request, handler, context, m_executor.get());
}

UntagResourceOutcome AppSyncClient::UntagResource(const UntagResourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ResourceArnHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: ResourceArn, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ResourceArn]", false));
  }
  if (!request.TagKeysHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UntagResource", "Required field: TagKeys, is not set");
    return UntagResourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TagKeys]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UntagResource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/tags/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetResourceArn());
  return UntagResourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_DELETE, Aws::Auth::SIGV4_SIGNER));
}

UntagResourceOutcomeCallable AppSyncClient::UntagResourceCallable(const UntagResourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UntagResource, request, m_executor.get());
}

void AppSyncClient::UntagResourceAsync(const UntagResourceRequest& request, const UntagResourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UntagResource, request, handler, context, m_executor.get());
}

UpdateApiCacheOutcome AppSyncClient::UpdateApiCache(const UpdateApiCacheRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateApiCache", "Required field: ApiId, is not set");
    return UpdateApiCacheOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateApiCache, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/ApiCaches/update");
  return UpdateApiCacheOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateApiCacheOutcomeCallable AppSyncClient::UpdateApiCacheCallable(const UpdateApiCacheRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateApiCache, request, m_executor.get());
}

void AppSyncClient::UpdateApiCacheAsync(const UpdateApiCacheRequest& request, const UpdateApiCacheResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateApiCache, request, handler, context, m_executor.get());
}

UpdateApiKeyOutcome AppSyncClient::UpdateApiKey(const UpdateApiKeyRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateApiKey", "Required field: ApiId, is not set");
    return UpdateApiKeyOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.IdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateApiKey", "Required field: Id, is not set");
    return UpdateApiKeyOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Id]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateApiKey, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/apikeys/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetId());
  return UpdateApiKeyOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateApiKeyOutcomeCallable AppSyncClient::UpdateApiKeyCallable(const UpdateApiKeyRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateApiKey, request, m_executor.get());
}

void AppSyncClient::UpdateApiKeyAsync(const UpdateApiKeyRequest& request, const UpdateApiKeyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateApiKey, request, handler, context, m_executor.get());
}

UpdateDataSourceOutcome AppSyncClient::UpdateDataSource(const UpdateDataSourceRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDataSource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDataSource", "Required field: ApiId, is not set");
    return UpdateDataSourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.NameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDataSource", "Required field: Name, is not set");
    return UpdateDataSourceOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [Name]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDataSource, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/datasources/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetName());
  return UpdateDataSourceOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateDataSourceOutcomeCallable AppSyncClient::UpdateDataSourceCallable(const UpdateDataSourceRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateDataSource, request, m_executor.get());
}

void AppSyncClient::UpdateDataSourceAsync(const UpdateDataSourceRequest& request, const UpdateDataSourceResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateDataSource, request, handler, context, m_executor.get());
}

UpdateDomainNameOutcome AppSyncClient::UpdateDomainName(const UpdateDomainNameRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.DomainNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateDomainName", "Required field: DomainName, is not set");
    return UpdateDomainNameOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [DomainName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateDomainName, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/domainnames/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetDomainName());
  return UpdateDomainNameOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateDomainNameOutcomeCallable AppSyncClient::UpdateDomainNameCallable(const UpdateDomainNameRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateDomainName, request, m_executor.get());
}

void AppSyncClient::UpdateDomainNameAsync(const UpdateDomainNameRequest& request, const UpdateDomainNameResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateDomainName, request, handler, context, m_executor.get());
}

UpdateFunctionOutcome AppSyncClient::UpdateFunction(const UpdateFunctionRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateFunction", "Required field: ApiId, is not set");
    return UpdateFunctionOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.FunctionIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateFunction", "Required field: FunctionId, is not set");
    return UpdateFunctionOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FunctionId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateFunction, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/functions/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFunctionId());
  return UpdateFunctionOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateFunctionOutcomeCallable AppSyncClient::UpdateFunctionCallable(const UpdateFunctionRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateFunction, request, m_executor.get());
}

void AppSyncClient::UpdateFunctionAsync(const UpdateFunctionRequest& request, const UpdateFunctionResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateFunction, request, handler, context, m_executor.get());
}

UpdateGraphqlApiOutcome AppSyncClient::UpdateGraphqlApi(const UpdateGraphqlApiRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateGraphqlApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateGraphqlApi", "Required field: ApiId, is not set");
    return UpdateGraphqlApiOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateGraphqlApi, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  return UpdateGraphqlApiOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateGraphqlApiOutcomeCallable AppSyncClient::UpdateGraphqlApiCallable(const UpdateGraphqlApiRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateGraphqlApi, request, m_executor.get());
}

void AppSyncClient::UpdateGraphqlApiAsync(const UpdateGraphqlApiRequest& request, const UpdateGraphqlApiResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateGraphqlApi, request, handler, context, m_executor.get());
}

UpdateResolverOutcome AppSyncClient::UpdateResolver(const UpdateResolverRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateResolver, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateResolver", "Required field: ApiId, is not set");
    return UpdateResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.TypeNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateResolver", "Required field: TypeName, is not set");
    return UpdateResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TypeName]", false));
  }
  if (!request.FieldNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateResolver", "Required field: FieldName, is not set");
    return UpdateResolverOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [FieldName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateResolver, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTypeName());
  endpointResolutionOutcome.GetResult().AddPathSegments("/resolvers/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetFieldName());
  return UpdateResolverOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateResolverOutcomeCallable AppSyncClient::UpdateResolverCallable(const UpdateResolverRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateResolver, request, m_executor.get());
}

void AppSyncClient::UpdateResolverAsync(const UpdateResolverRequest& request, const UpdateResolverResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateResolver, request, handler, context, m_executor.get());
}

UpdateTypeOutcome AppSyncClient::UpdateType(const UpdateTypeRequest& request) const
{
  AWS_OPERATION_CHECK_PTR(m_endpointProvider, UpdateType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE);
  if (!request.ApiIdHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateType", "Required field: ApiId, is not set");
    return UpdateTypeOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [ApiId]", false));
  }
  if (!request.TypeNameHasBeenSet())
  {
    AWS_LOGSTREAM_ERROR("UpdateType", "Required field: TypeName, is not set");
    return UpdateTypeOutcome(Aws::Client::AWSError<AppSyncErrors>(AppSyncErrors::MISSING_PARAMETER, "MISSING_PARAMETER", "Missing required field [TypeName]", false));
  }
  ResolveEndpointOutcome endpointResolutionOutcome = m_endpointProvider->ResolveEndpoint(request.GetEndpointContextParams());
  AWS_OPERATION_CHECK_SUCCESS(endpointResolutionOutcome, UpdateType, CoreErrors, CoreErrors::ENDPOINT_RESOLUTION_FAILURE, endpointResolutionOutcome.GetError().GetMessage());
  endpointResolutionOutcome.GetResult().AddPathSegments("/v1/apis/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetApiId());
  endpointResolutionOutcome.GetResult().AddPathSegments("/types/");
  endpointResolutionOutcome.GetResult().AddPathSegment(request.GetTypeName());
  return UpdateTypeOutcome(MakeRequest(request, endpointResolutionOutcome.GetResult(), Aws::Http::HttpMethod::HTTP_POST, Aws::Auth::SIGV4_SIGNER));
}

UpdateTypeOutcomeCallable AppSyncClient::UpdateTypeCallable(const UpdateTypeRequest& request) const
{
  AWS_MAKE_CALLABLE_OPERATION(UpdateType, request, m_executor.get());
}

void AppSyncClient::UpdateTypeAsync(const UpdateTypeRequest& request, const UpdateTypeResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  AWS_MAKE_ASYNC_OPERATION(UpdateType, request, handler, context, m_executor.get());
}

