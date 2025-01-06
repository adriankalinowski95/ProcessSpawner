import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProcessEventListComponent } from './process-event-list.component';

describe('ProcessEventListComponent', () => {
  let component: ProcessEventListComponent;
  let fixture: ComponentFixture<ProcessEventListComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ProcessEventListComponent]
    })
    .compileComponents();
    
    fixture = TestBed.createComponent(ProcessEventListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
