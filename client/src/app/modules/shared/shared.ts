import * as BaseResponse from "./response/base-response"
import * as BasePaginationResponse from "./response/base-pagination-response"
import * as BaseFieldValidationResponse from "./response/base-field-validation-response"
import * as ObjectOperationResponse from "./response/object-operation-response"
import * as ObjectResponse from "./response/object-response"
import * as BasePaginationRequest from "./request/base-pagination-request"
import * as FormErrorHandlingService from "./utils/form-error-handling"
import * as Base64Conv from "./utils/base64-converter"
import * as IsNotNullOrUnd from "./utils/validators"
import * as CommonMeth from "./utils/common-methods"
import * as CommonDefinitions from "./definitions"
import * as _BaseCrudService from "./request/base-crud-service"
import * as _ExtendCrudService from "./request/extend-crud-service"
import * as _ObjectsResponse from "./response/objects-response"
import * as _NotificationService from "./services/notification.service"

export namespace shared {
    export namespace response {
        export import Base = BaseResponse.BaseResponse;
        export import BasePagination = BasePaginationResponse.BasePaginationResponse;
        export import BaseFieldValidation = BaseFieldValidationResponse.BaseFieldValidationResponse;
        export import Object = ObjectResponse.ObjectResponse;
        export import Objects = _ObjectsResponse.ObjectsResponse;
        export import ObjectOperation = ObjectOperationResponse.ObjectOperationResponse;
    }

    export namespace request {
        export import BasePagination = BasePaginationRequest.BasePaginationRequest;
    }

    export namespace enums {
        export import BaseResponseStatus = BaseResponse.BaseResponseStatus;
    }

    export namespace services {
        export import FormErrorHandling = FormErrorHandlingService.FormErrorHandlingService;
        export import BaseCrudService = _BaseCrudService.BaseCrudService;
        export import ExtendCrudService = _ExtendCrudService.ExtendCrudService;
        export import NotificationService = _NotificationService.NotificationService;
    }

    export import definitions = CommonDefinitions.definitions
    export import isNotNullOrUndefined = IsNotNullOrUnd.isNotNullOrUndefined;
    export import Base64Converter = Base64Conv.Base64Converter;
    export import getRandomString = CommonMeth.getRandomString;
    export import getRandomNumber = CommonMeth.getRandomNumber;
}