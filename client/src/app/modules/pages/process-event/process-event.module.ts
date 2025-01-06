import { CommonModule } from "@angular/common";
import { ProcessEventListComponent } from "./components/process-event-list/process-event-list.component";
import { ProcessEventRoutingModule } from "./process-event-routing.module";
import { FormsModule, ReactiveFormsModule } from "@angular/forms";
import { MatInputModule } from "@angular/material/input";
import { HttpClientModule } from "@angular/common/http";
import { MatFormFieldModule } from "@angular/material/form-field";
import { MatButtonModule } from "@angular/material/button";
import { MatCardModule } from "@angular/material/card";
import { MatIconModule } from "@angular/material/icon";
import { ElementListModule } from "../../shared/element-list/element-list.module";
import { NgModule } from "@angular/core";

@NgModule({
  declarations: [
    ProcessEventListComponent
  ],
  imports: [
    CommonModule,
    ProcessEventRoutingModule,
    FormsModule,
    ReactiveFormsModule,
    HttpClientModule,
    MatInputModule,
    MatFormFieldModule,
    MatButtonModule,
    MatCardModule,
    MatIconModule,
    ElementListModule
  ]
})
export class ProcessEventModule { }
