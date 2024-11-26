import { BaseFieldValidationResponse } from "./base-field-validation-response";
import { ObjectResponse } from "./object-response";

export interface ObjectOperationResponse<T> extends ObjectResponse<T> {
    validationResult: BaseFieldValidationResponse[];
}