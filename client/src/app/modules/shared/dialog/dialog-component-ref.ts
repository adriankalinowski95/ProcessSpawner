import { Observable } from "rxjs/internal/Observable";

export interface DialogComponentRef {
  data: any;
  outputData: any;
  validate?: () => boolean;
  validateAsync?: () => Observable<boolean>
}