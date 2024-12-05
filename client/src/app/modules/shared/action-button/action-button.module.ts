import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import {MatButtonModule} from '@angular/material/button';
import {ActionButtonComponent} from "./action-button.component";



@NgModule({
  declarations: [ActionButtonComponent],
  imports: [
    CommonModule,
    MatButtonModule
  ],
  exports: [ActionButtonComponent]
})
export class ActionButtonModule { }
